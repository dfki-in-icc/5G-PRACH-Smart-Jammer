/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */
 
#include "prometheus_exporter.h"
#include "executables/softmodem-common.h"

prom_metrics_t  prom_metrics;
complex_metrics_t complex_metrics;
uint8_t  Prom_switch_state[NUM_PROM_SETTINGS];
uint8_t  Realtime_switch_state[NUM_REALTIME_SETTINGS];
pthread_mutex_t mutex_prom;
pthread_mutex_t mutex_real;

int parseHeader(char* in, char* Query, char* path, char* proto);
int SendResponse(int sock, const char* msg, char* context);
int GetSettings(int sock, const char* msg, char* inbuf);
int SendSettings(int sock, const char* msg);
int getMethodIndex(char* QueryType);

const char* PromMetricSwitchNames[] = {
  // prometheus monitor
  "pdcch_pow",  "dl_freq_resource",  "ul_freq_resource",  "dl_mcs",  "dl_tbs",  "ul_mcs",  "ul_tbs",  "pss_peak_power",
  };
const char* RealtimeMetricSwitchNames[] = {
  // real time monitor
  "pbch_iq",  "pbch_ch",  "pdcch_iq",  "pdcch_ch",  "rar_iq",  "rar_ch",  "sib_iq",  "sib_ch", "sss_iq", 
  "pdsch_iq", "pdsch_chest"
  };

const char* HttpMethod[]={  "GET","POST","PUT" };
void *PrometheusNodeExporter_thread(void* arg){
  int sock0;
  struct sockaddr_in addr;
  struct sockaddr_in client;
  char buf[4096];
  char inbuf[2048];
  char inbuf_store[2048];
  int len;
  int sock;
  int yes = 1;
  struct tm  *time_inf;
  char context[65536];
  char tm_buf[256];
  char cdate[512];
  char tmp_str[64];
  const char* msg1 = "HTTP/1.0 200 OK\r\n";
  const char* msg2 = "Content-Type: application/octet-stream\r\n";
  const char* msg_200  = "HTTP/1.0 200 OK\r\n";
  const char* msg_type = "Content-Type: application/octet-stream\r\n";
  const char* msg_cors = "Access-Control-Allow-Origin: *\r\n";
  const char* msg_400  = "HTTP/1.0 400 Bad Request\r\n";
  const char* msg_401  = "HTTP/1.0 401 Unauthorized\r\n";
  const char* msg_403  = "HTTP/1.0 403 Forbidden\r\n";
  const char* msg_404  = "HTTP/1.0 404 Not Found\r\n";
  const char* msg_405  = "HTTP/1.0 405 Method Not Allowed\r\n";

  char QueryType[256];
  char DirPath[256];
  char Proto[256];

  sock0 = socket(AF_INET, SOCK_STREAM, 0);
  if (sock0 < 0) {
	  perror("socket");
	  return 1;
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(prometheus_port);
  addr.sin_addr.s_addr = INADDR_ANY;

  setsockopt(sock0, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));

  if (bind(sock0, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
 	  perror("bind");
 	  return 1;
  }

  if (listen(sock0, 5) != 0) {
	  perror("listen");
	  return 1;
  }

  while (1) {
    len = sizeof(client);
    sock = accept(sock0, (struct sockaddr *)&client, &len);
    if (sock < 0) {
	    perror("accept");
	    break;
    }

    memset(inbuf, 0, sizeof(inbuf));
    recv(sock, inbuf, sizeof(inbuf), 0);
    strcpy(inbuf_store,inbuf);

    // get query , directry path , protocol
    parseHeader(inbuf, QueryType,DirPath, Proto);
    // supported query ?
    if (getMethodIndex(QueryType) < 0){
      printf("un-supported query type : %s\n",QueryType);
    }

    int metrics_to_monitor = 0;
    if (strstr(QueryType,"GET") != NULL){
      // prometheus interface '/metrics'
      if (strstr(DirPath,"metrics") != NULL){
        // printf("requsted metrics\n");
        metrics_to_monitor = PROMETHEUS;
      }

      // PBCH '/iq/pbch
      if (strstr(DirPath,"iq/pbch") != NULL){
        // printf("requsted iq/pbch\n");
        metrics_to_monitor = PBCH_IQ;
      }
      // PBCH Channel Estimation '/chest/pbch
      if (strstr(DirPath,"chest/pbch") != NULL){
        // printf("requsted chest/pbch\n");
        metrics_to_monitor = PBCH_CHEST;
      }

      // SI   '/iq/sib
      if (strstr(DirPath,"iq/sib") != NULL){
        // printf("requsted iq/sib\n");
        metrics_to_monitor = SIB_IQ;
      }
      // SI Channel Estimation  '/chest/sib
      if (strstr(DirPath,"chest/sib") != NULL){
        // printf("requsted iq/sib\n");
        metrics_to_monitor = SIB_CHEST;
      }

      // pdcch  '/iq/pdcch
      if (strstr(DirPath,"iq/pdcch") != NULL){
        // printf("requsted iq/pdcch\n");
        metrics_to_monitor = PDCCH_IQ;
      }
      // pdcch Channel Estimation '/chest/pdcch
      if (strstr(DirPath,"chest/pdcch") != NULL){
        // printf("requsted iq/pdcch\n");
        metrics_to_monitor = PDCCH_CHEST;
      }

      // RAR  '/iq/rar
      if (strstr(DirPath,"iq/rar") != NULL){
        // printf("requsted iq/pdcch\n");
        metrics_to_monitor = RAR_IQ;
      }
      // RAR Channel Estimation '/chest/rar
      if (strstr(DirPath,"chest/rar") != NULL){
        // printf("requsted iq/pdcch\n");
        metrics_to_monitor = RAR_CHEST;
      }
      // SSS Compensated I/Q '/iq/sss
      if (strstr(DirPath,"iq/sss") != NULL){
        // printf("requsted iq/pdcch\n");
        metrics_to_monitor = SSS_IQ;
      }
      // PDSCH Channel '/chest/pdsch
      if (strstr(DirPath,"chest/pdsch") != NULL){
        // printf("requsted iq/pdcch\n");
        metrics_to_monitor = PDSCH_CHEST;
      }
      // PDSCH Compensated I/Q '/iq/pdsch
      if (strstr(DirPath,"iq/pdsch") != NULL){
        // printf("requsted iq/pdcch\n");
        metrics_to_monitor = PDSCH_IQ;
      }
      // Get Current Settings
      if (strstr(DirPath,"settings") != NULL){
        metrics_to_monitor = SETTINGS;
      }

      if (metrics_to_monitor == PROMETHEUS){
        bool bTop = true;
        strcpy(tmp_str,"");
        for (int i=0;i<NUM_PROM_ELEMENTS;i++){
          if ( strcmp(prom_metrics.metric_element[i].metric_name,"not_set")!=0 ){
            sprintf(tmp_str,"%s   %d\n",
              prom_metrics.metric_element[i].metric_name,
              prom_metrics.metric_element[i].metric_value);
          } else {
            strcpy(tmp_str,"");
          }
          if (bTop){
            strcpy(context,tmp_str);
            bTop = false;
          } else {
            strcat(context,tmp_str);
          }
        }
      } else {  // complex data (I/Q)
        if (metrics_to_monitor != SETTINGS){
          strcpy(buf,"");
          Report_IQ_Metric(buf,&complex_metrics.complex_element[metrics_to_monitor]);
          strcpy(context,buf);
        } else {
          char locBuf[256];
          strcpy(context,"");
          for (int i=0;i<NUM_REALTIME_SETTINGS;i++){
            sprintf(locBuf,"%s=%d\n",RealtimeMetricSwitchNames[i],Realtime_switch_state[i]);
            strcat(context,locBuf);
          }
          for (int i=0;i<NUM_PROM_SETTINGS;i++){
            sprintf(locBuf,"%s=%d\n",PromMetricSwitchNames[i],Prom_switch_state[i]);
            strcat(context,locBuf);
          }
        }
      } 

      if (metrics_to_monitor == SETTINGS){
        SendSettings(sock, msg_200);
      } else {
        SendResponse(sock, msg_200, context);
      }
    } else if (strstr(QueryType,"POST") != NULL){  // response to post method
      strcpy(context,"");
      GetSettings(sock, msg_200, inbuf_store);
    }
  }
}

struct thdata       thdata[ NUM_MAX_COMPLEX_ELEMENTS ];
void *cp_thread(void* arg){
  struct thdata* pth = (struct thdata*) arg;
  while(1){
      /* sync */
    sem_post(&pth->sync);
    sem_wait(&pth->start);

    if (Realtime_switch_state[pth->index] != 0){
      strcpy(complex_metrics.complex_element[pth->index].label,pth->label);
      pthread_mutex_lock(&mutex_real);
      memcpy(complex_metrics.complex_element[pth->index].iq_data, pth->pdata, sizeof(int32_t)*pth->size);
      pthread_mutex_unlock(&mutex_real);
      complex_metrics.complex_element[pth->index].size = pth->size;
    }

    /* sync */
    sem_post(&pth->sync);
  }

  /* done */
  return (void *) NULL;
}

void init_PrometheusNodeExporter_thread(void* arg){
    pthread_t thread_prom;
    char ThreadName[128][ NUM_MAX_COMPLEX_ELEMENTS ];
    for (int i=0;i<NUM_PROM_ELEMENTS;i++){
      strcpy(prom_metrics.metric_element[i].metric_name,"not_set");
      prom_metrics.metric_element[i].metric_value = 0;
    }

    pthread_mutex_init(&mutex_prom, NULL);
    pthread_mutex_init(&mutex_real, NULL);

    MonitoringConfig();

    prom_metrics.num_metrics = 0;
    threadCreate(
      &thread_prom, PrometheusNodeExporter_thread /* function */, (void *) arg,
	    "PrometheusNodeExporter_thread" /* name */, -1 /* affinity */, SCHED_OAI /* priority */);
      pthread_setname_np(thread_prom, "Prome_thread");

    for (int i=0;i< NUM_MAX_COMPLEX_ELEMENTS; i++){
      thdata[i].index = i;
      // printf("copy thread create %d\n",i);
      sem_init(&thdata[i].sync, 0, 0);
      sem_init(&thdata[i].start, 0, 0);
      if (pthread_create(
        &thdata[i].th, NULL, cp_thread,  (void*) &thdata[i]) != 0) {
        printf("pthread_create failed ");
      }
      sprintf(ThreadName,"PrmCP_Thread_%d",i);
      pthread_setname_np(thdata[i].th, ThreadName);
    }

    printf("[PHY] PrometheusNodeExporter_thread created ( %s )\n",__FUNCTION__);
}

int RegisterMetric(uint32_t key, char* name, uint32_t value){
  int ret_code = -1;
//  if (Prom_switch_state[key] == 0) return 1;  // if monitoring operation disabled, skip this function 

  if (key < NUM_MAX_PROM_ELEMENTS){
    pthread_mutex_lock(&mutex_prom);
    strcpy(prom_metrics.metric_element[key].metric_name,name);
    prom_metrics.metric_element[key].metric_value = value;
    pthread_mutex_unlock(&mutex_prom);
    ret_code = 0;
  }
  return ret_code;
}

int RegisterComplexMetric(uint32_t key, char* label, int16_t* ptr, uint32_t size){
  int ret_code = -1;
  if (Realtime_switch_state[key] == 0) return 1;  // if monitoring operation disabled, skip this function 

  if (key < NUM_MAX_COMPLEX_ELEMENTS){
    if (Realtime_switch_state[key] == 0) return ret_code;
#if 0
    strcpy(complex_metrics.complex_element[key].label,label);
    pthread_mutex_lock(&mutex_real);
    memcpy(complex_metrics.complex_element[key].iq_data, ptr, sizeof(int32_t)*size);
    pthread_mutex_unlock(&mutex_real);
    complex_metrics.complex_element[key].size = size;
#endif
    thdata[key].pdata = ptr;
    thdata[key].size = size;
    strcpy(thdata[key].label,label);
    sem_post(&thdata[key].start);
    ret_code = 0;
  }
  return ret_code;
}

int Report_IQ_Metric(char* buf,complex_metric_elm_t* elm){
    char tmp_buf[8192];
    char name[256];

    for (int i = 0 ; i < elm->size * 2 ; i += 2){
      sprintf(tmp_buf,"%d,%d\n",
            elm->iq_data[i],elm->iq_data[i+1]);
      strcat(buf,tmp_buf);
    }

    return strlen(buf);
}

int parseHeader(char* in, char* Query, char* path, char* proto){
  char* ptr;
  char* firstLine;
  char* ptrLine1;
  char* lastLine;

  int length = strlen(in);

  // 1st line
  ptr = strtok(in,"\n"); if (ptr == NULL) return -1;
  //printf("(1) %s\n",ptr);
  firstLine = ptr;
  // Method
  ptrLine1 = strtok(firstLine," "); if (ptrLine1 == NULL) return -1;
  strcpy(Query,ptrLine1);
  // path
  ptrLine1 = strtok(NULL," "); if (ptrLine1 == NULL) return -1;
  strcpy(path,ptrLine1);
  // protocol
  ptrLine1 = strtok(NULL," "); if (ptrLine1 == NULL) return -1;
  strcpy(proto,ptrLine1);

  return length;
}

int SendResponse(int sock, const char* msg, char* context){
  char buf[16384];
  char tm_buf[256];
  char cdate[512];

  const char* msg_type = "Content-Type: application/octet-stream\r\n";
  const char* msg_cors = "Access-Control-Allow-Origin: *\r\n";
  time_t timep = time(NULL);
  struct tm  *time_inf = localtime(&timep);
  strftime(tm_buf,sizeof(tm_buf),"%c", time_inf);
  sprintf(cdate,"Date: %s\r\n",tm_buf);

  memset(buf, 0, sizeof(buf));
  snprintf(buf,sizeof(buf),
    "%s%s%s%s\r\n%s",
       msg, cdate, msg_type, msg_cors, context);
  //  send message
  send(sock, buf, (int)strlen(buf), 0);
  close(sock);
  // printf("%s\n",buf);
}

int GetSettingHTML(char* buf2){
  char tmp_buf[256];
  // Setting Result
  sprintf(buf2,"<html>\n");
  sprintf(buf2,"%s<body>\n",buf2);
  sprintf(buf2,"%s<h1>Current Settings</h1><br>\n",buf2);
  sprintf(buf2,"%s<ol>\n",buf2);
  for (int i=0;i<NUM_REALTIME_SETTINGS;i++){
    sprintf(tmp_buf,"<li>%s=%d</li>\n",RealtimeMetricSwitchNames[i],Realtime_switch_state[i]);
    strcat(buf2,tmp_buf);
  }
  for (int i=0;i<NUM_PROM_SETTINGS;i++){
    sprintf(tmp_buf,"<li>%s=%d</li>\n",PromMetricSwitchNames[i],Prom_switch_state[i]);
    strcat(buf2,tmp_buf);
  }
  sprintf(buf2,"%s</ol>\n",buf2);
  sprintf(buf2,"%s</body>\n",buf2);
  strcat(buf2,"</html>");
}

int GetSettings(int sock, const char* msg, char* inbuf){
  char buf[4096];
  char buf2[4096];
  char tm_buf[256];
  char cdate[512];
  char* ptr;
  char* lastLine;

  const char* msg_type = "Content-Type: text/plain\r\n";
  const char* msg_cors = "Access-Control-Allow-Origin: *\r\n";
  time_t timep = time(NULL);
  struct tm  *time_inf = localtime(&timep);
  strftime(tm_buf,sizeof(tm_buf),"%c", time_inf);
  sprintf(cdate,"Date: %s\r\n",tm_buf);
  // Get posted parameter
  ptr = strtok(inbuf,"\n");
  while (ptr != NULL){
    lastLine = ptr;
    ptr = strtok(NULL,"\n");
  }
  // parse parameter
  for (int i=0;i<NUM_PROM_SETTINGS;i++){
    if (strstr(lastLine,PromMetricSwitchNames[i]) != NULL) Prom_switch_state[i] = 1;
    else  Prom_switch_state[i] = 0;
  }
  for (int i=0;i<NUM_REALTIME_SETTINGS;i++){
    if (strstr(lastLine,RealtimeMetricSwitchNames[i]) != NULL) Realtime_switch_state[i] = 1;
    else  Realtime_switch_state[i] = 0;
  }

  GetSettingHTML(buf2);

  memset(buf, 0, sizeof(buf));
  snprintf(buf,sizeof(buf),
    "%s%s%s\r\n%s",
       msg, cdate, msg_cors,buf2);
  // 送信
  send(sock, buf, (int)strlen(buf), 0);
  close(sock);
  // printf("]]]]]]\n%s\n]]]]]]]\n",buf);
}

int SendSettings(int sock, const char* msg){
  char buf[4096];
  char buf2[4096];
  char tm_buf[256];
  char cdate[512];
  char* ptr;
  char* lastLine;

  const char* msg_type = "Content-Type: text/plain\r\n";
  const char* msg_cors = "Access-Control-Allow-Origin: *\r\n";
  time_t timep = time(NULL);
  struct tm  *time_inf = localtime(&timep);
  strftime(tm_buf,sizeof(tm_buf),"%c", time_inf);
  sprintf(cdate,"Date: %s\r\n",tm_buf);

  GetSettingHTML(buf2);

  memset(buf, 0, sizeof(buf));
  snprintf(buf,sizeof(buf),
    "%s%s%s\r\n%s",
       msg, cdate, msg_cors,buf2);
  // 送信
  send(sock, buf, (int)strlen(buf), 0);
  close(sock);
  // printf("]]]]]]\n%s\n]]]]]]]\n",buf);
}

int getMethodIndex(char* QueryType){
  for (int i=0;i<NUM_METHOD;i++){
    if (strcmp(HttpMethod[i],QueryType)==0){
      // printf("matched : %s\n",QueryType);
      return i;
    }
  }
  return -1;
}

int MonitoringConfig(){
  FILE* fp;
  char buf[256];
  int ret_code = 0;

  if ((fp = fopen("./monitor.conf","r")) != NULL){
    printf("Monitring point is set to default.\n");
  }
  printf("Monitring point is loaded from monitor.conf\n");
  while (fgets(buf, 256, fp) != NULL) {
    if (strstr(buf,"//") == NULL){
      //
      for (int i=0;i<NUM_REALTIME_SETTINGS;i++){
        if (strstr(buf,RealtimeMetricSwitchNames[i]) != NULL){
          Realtime_switch_state[i] = 1;
          // printf("%s : %s ",RealtimeMetricSwitchNames[i],buf);
        }
      for (int i=0;i<NUM_PROM_SETTINGS;i++){
        // printf("%s : %s ",PromMetricSwitchNames[i],buf);
        if (strstr(buf,PromMetricSwitchNames[i]) != NULL){
          Prom_switch_state[i] = 1;
        //  printf("-----> %s\n",PromMetricSwitchNames[i]);
        }
      }
      }
    }
  }
  fclose(fp);

  return ret_code;
}