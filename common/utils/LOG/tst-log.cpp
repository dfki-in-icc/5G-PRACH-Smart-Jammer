/*
 * unit tests to evaluate integration of Log library.
 *
 * Example of running commandline: ./tstlog [--run_test=testname] [BOOST Test options]
 * I.e.: ./tstlog --run_test=test_log_rotation_1 --report_level=detailed
 */

#define BOOST_TEST_MODULE tstlog
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <fstream>

#undef T_TRACER
#include "log.h"
#include "common/config/config_load_configmodule.h"
using namespace boost::unit_test;

volatile int oai_exit;//the definition of this variable is needed by log.c:flush_mem_to_file() and it is declared in log.c
const char *global_oai_logfile="/tmp/openair.log";
const char *hw_logfile="/tmp/HW.";
const char *phy_logfile="/tmp/PHY.log";
std::list<int> known_bugged_comp = {19, 24, 40};
std::string conf1="\
log_config :\n\
{\n\
    global_log_level   = \"debug\";\n\
    hw_log_level       = \"info\";\n\
    global_log_options = (\"level\",\"nocolor\",\"time\");\n\
    global_log_rotation_size = 1;\n\
    global_log_rotation_time = 1;\n\
};\n";

std::string conf2="\
log_config :\n\
{\n\
    global_log_options = (\"level\",\"nocolor\",\"time\");\n\
    global_log_rotation_size = 1;\n\
    global_log_rotation_time = 1;\n\
};\n";

std::string conf3="\
log_config :\n\
{\n\
    global_log_options = (\"level\",\"nocolor\",\"time\", \"correlation_id\");\n\
    global_log_rotation_size = 0;\n\
    global_log_rotation_time = 0;\n\
};\n";

std::string conf4="\
log_config :\n\
{\n\
  global_log_level                      =\"info\";\n\
  global_log_infile                     = 1; # warn: this parameter is silently discarded from configuration parser \n\
  global_log_online                     = 0;\n\
};\n";
#define TMPLOCALCONF "tstlog.conf.tmp"
const char *argv[4] = {"tstlog", "-O", TMPLOCALCONF, NULL};
int argc=3;

void log_comp_tostr(unsigned int i) {
  std::cout << "Component [" << i << "- " << g_log->log_component[i].name << "]: stream "<< g_log->log_component[i].stream
            << ", level:" << g_log->log_component[i].level << ", filelog: " << g_log->log_component[i].filelog << std::endl;
}
bool search_in_file(std::string fname, std::string match) {
  std::ifstream f(fname);

  if (!f)
    return false;

  std::string t;

  while (getline(f, t))
    if (t.find(match, 0) != t.npos)
      return true;

  f.close();
  return false;
}

void create_test_conf_file(std::string confcontent) {
  std::remove(TMPLOCALCONF);
  std::ofstream file(TMPLOCALCONF);
  file << confcontent;
}

BOOST_AUTO_TEST_CASE(test_log_rotation_1) {
  // This test will be using global log file (/tmp/openair.log), simulating time expiring for log rotation.
  // It is a test for single thread logging
  create_test_conf_file(conf1);
  std::remove(global_oai_logfile);
  std::remove(hw_logfile);
  BOOST_TEST_REQUIRE ((load_configmodule(argc, const_cast<char **>(argv), 0) != NULL), "Configuration module init failed");
  BOOST_TEST ((logInit() == 0), true);
  /*
   * Sanity check for some field of global structure of log API.
   * Expected: all components log to DEBUG level on global file, except for component 28 (HW) that will have its file and log at info level
   */
  BOOST_TEST (((g_log->global_logfile == 0) && (g_log->global_logfilep == NULL)), true);

  for (int i=MIN_LOG_COMPONENTS; i < MAX_LOG_PREDEF_COMPONENTS; i++) {
    auto itr = std::find(known_bugged_comp.begin(), known_bugged_comp.end(), i);

    if (itr != known_bugged_comp.end())
      continue;

    bool sts = ((g_log->log_component[i].filelog == 1)   &&
                (g_log->log_component[i].name != NULL)   &&
                (g_log->log_component[i].stream != g_log->global_logfilep));
    sts = (i == 28) ? sts && (g_log->log_component[i].level == OAILOG_INFO) : sts && (g_log->log_component[i].level == OAILOG_DEBUG);

    if (sts == false) {
      std::cout << "Failure at this component:";
      log_comp_tostr(i);
    }

    BOOST_TEST (sts, true);
  }

  BOOST_TEST((set_glog(OAILOG_DEBUG) == 0), true);
  BOOST_TEST((set_glog_filelog(true) == 0), true); // force dump to default logfile /tmp/openair.log.
  BOOST_TEST (((g_log->global_logfile == 1) && (g_log->global_logfilep != NULL)), true);
  LOG_I(HW, "First test for a string");
  BOOST_TEST(set_glog(OAILOG_ERR) == 0, true);
  LOG_I(HW, "You should not see this line");
  BOOST_TEST((set_glog(OAILOG_DEBUG) == 0), true);
  char string_truncation[1600];
  memset(string_truncation, 's', 1599);
  memset(&string_truncation[1599], '\0', 1);
  LOG_I(HW, "%s", string_truncation);
  sleep(5);// let's have log rotation
  LOG_I(HW, "Spurious line terminator should not appear\n");
  bool sts = search_in_file(hw_logfile, "\n");

  if (sts == true) {
    std::cout << "Offending line:";
    std::ifstream f(hw_logfile);

    if (f.is_open())
      std::cout << f.rdbuf();
  }

  BOOST_TEST(sts == false);
  logClean();
  logTerm();
  end_configmodule();
  std::remove(TMPLOCALCONF);
}

BOOST_AUTO_TEST_CASE(test_log_rotation_2) {
  // test using component log file (tmp/PHY.log), simulating time expiring for log rotation
  // It is a test for single thread logging
  create_test_conf_file(conf1);
  std::remove(phy_logfile);
  BOOST_TEST_REQUIRE ((load_configmodule(argc, const_cast<char **>(argv), 0) != NULL), "Configuration module init failed");
  BOOST_TEST (logInit() == 0, true);
  BOOST_TEST(set_glog(OAILOG_DEBUG) == 0, true);
  BOOST_TEST(set_component_filelog(0) == 0, true);
  LOG_I(PHY, "Second test for a string that should be moved to backup file");
  sleep(5);
  LOG_I(PHY, "this should go to a brand new file");
  BOOST_TEST( search_in_file(phy_logfile, "this should go to a brand new file") == true);
  BOOST_TEST( search_in_file(phy_logfile, "Second test for a string that should be moved to backup file") == false);
  logClean();
  logTerm();
  end_configmodule();
  std::remove(TMPLOCALCONF);
}

BOOST_AUTO_TEST_CASE(test_log_defaults) {
  // testing log default values. You should not see anylog, not on console, not on file. It is a test for single thread logging
  create_test_conf_file(conf2);
  std::remove(phy_logfile);
  BOOST_TEST_REQUIRE ((load_configmodule(argc, const_cast<char **>(argv), 0) != NULL), "Configuration module init failed");
  BOOST_TEST (logInit() == 0, true);
  LOG_I(PHY, "you should not see any log");
  BOOST_TEST( search_in_file(phy_logfile, "you should not see any log") == false);
  logClean();
  logTerm();
  end_configmodule();
  std::remove(TMPLOCALCONF);
}

BOOST_AUTO_TEST_CASE(test_log_defaults_2) {
  /* Test log default values. You should not see anylog, not on console, not on file. It is a test for single thread logging */
  create_test_conf_file(conf3);
  std::remove(phy_logfile);
  BOOST_TEST_REQUIRE ((load_configmodule(argc, const_cast<char **>(argv), 0) != NULL), "Configuration module init failed");
  BOOST_TEST (logInit() == 0, true);
  BOOST_TEST(set_log(0, OAILOG_INFO) == 0, true);
  LOG_I(PHY, "you should see this log to file");
  BOOST_TEST( search_in_file(phy_logfile, "you should see this log to file") == true);
  LOG_I(HW, "you should not see this log at all");
  //XXX: TODO: intercept console output and general log file and validate this finding with BOOST_TEST() macro.
  logClean();
  logTerm();
  end_configmodule();
  std::remove(TMPLOCALCONF);
}

BOOST_AUTO_TEST_CASE(test_log_discarded_options) {
  /* Test global log activation to file. You are expected to see only logs in their component file.
   * To have logs to just one global file you need to call set_glog_filelog().
   */
  create_test_conf_file(conf4);
  std::remove(phy_logfile);
  std::remove(global_oai_logfile);
  BOOST_TEST_REQUIRE ((load_configmodule(argc, const_cast<char **>(argv), 0) != NULL), "Configuration module init failed");
  BOOST_TEST(logInit() == 0, true);
  /*
   * Sanity check for some field of global structure of log API.
   * Expected: all components log to DEBUG level and have their own file sink
   */
  BOOST_TEST (((g_log->global_logfile == 0) && (g_log->global_logfilep == NULL)), true);

  for (int i=MIN_LOG_COMPONENTS; i < MAX_LOG_PREDEF_COMPONENTS; i++) {
    auto itr = std::find(known_bugged_comp.begin(), known_bugged_comp.end(), i);

    if (itr != known_bugged_comp.end())
      continue;

    bool sts = ((g_log->log_component[i].filelog == 1)   &&
                (g_log->log_component[i].name != NULL)   &&
                (g_log->log_component[i].stream != g_log->global_logfilep) &&
                (g_log->log_component[i].stream != NULL) &&
                (g_log->log_component[i].stream != stdout) &&
                (g_log->log_component[i].level == OAILOG_INFO));

    if (sts == false) {
      std::cout << "Failure at this component:";
      log_comp_tostr(i);
    }

    BOOST_TEST (sts, true);
  }

  LOG_I(PHY, "test_log_discarded_options");
  BOOST_TEST(search_in_file(phy_logfile, "test_log_discarded_options") == true);
  BOOST_TEST(search_in_file(global_oai_logfile, "test_log_discarded_options") == false);
  logClean();
  logTerm();
  end_configmodule();
  std::remove(TMPLOCALCONF);
}