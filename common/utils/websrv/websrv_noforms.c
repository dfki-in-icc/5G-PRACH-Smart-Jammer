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

/*! \file common/utils/websrv/websrv_noforms.c
 * \brief: primitives replacing xforms when compiling nr_phy_scope.c for the webserver shared lib
 * \author Francois TABURET
 * \date 2022
 * \version 0.1
 * \company NOKIA BellLabs France
 * \email: francois.taburet@nokia-bell-labs.com
 * \note
 * \warning
 */
#include <stdlib.h>
#include <jansson.h>
#include <ulfius.h>
#include "common/utils/LOG/log.h"
#include "common/utils/websrv/websrv.h"
#include "common/utils/websrv/websrv_noforms.h"


#define MAX_LAYER 8


/* replacement for xforms function used in xforms softscope */
typedef struct {             
websrv_scopedata_msg_t    *buf;                  
} websrv_scopedata_layermsgs_t;

typedef struct {
char                      * title;              /* overall title                */
websrv_scopedata_layermsgs_t    buff[MAX_LAYER];     /*  buff per layer */
int                       n[MAX_LAYER];         /* total points */                   
} FLI_XYPLOT_SPEC;


                                     
                                     
FL_OBJECT * websrv_fl_add_xyplot( int          t,
                              FL_Coord     x,
                              FL_Coord     y,
                              FL_Coord     w,
                              FL_Coord     h,
                              const char * label ){
    FL_OBJECT *obj;
    obj = calloc( 1, sizeof (FL_OBJECT) );
    AssertFatal(obj!=NULL,"Cannot allocate scope object for %s\n",label);
    obj->spec = calloc( 1, sizeof (FLI_XYPLOT_SPEC) );
    AssertFatal(obj->spec!=NULL,"Cannot allocate scope object buffers for %s\n",label);
    FLI_XYPLOT_SPEC *spec=(FLI_XYPLOT_SPEC *)obj->spec;
	for (int j=0; j<MAX_LAYER;j++) {
	  spec->buff[j].buf=(websrv_scopedata_msg_t *)calloc( 1, sizeof(websrv_scopedata_msg_t ) );
	}
    obj->objclass  = FL_XYPLOT;
    obj->type      = t;

    obj->x         = x;
    obj->y         = y;
    obj->w         = w;
    obj->h         = h;
    if (label)
      obj->label = strdup(label);
    LOG_I(UTIL,"[websrv], scope object for \"%s\" allocated obj at %p spec at %p\n",label,obj,obj->spec);

    return obj;

};

FL_OBJECT * websrv_fl_add_canvas( int          type,
                          FL_Coord     x,
                          FL_Coord     y,
                          FL_Coord     w,
                          FL_Coord     h,
                          const char * label ){
 return websrv_fl_add_xyplot(type, x,y,w,h,label);
}




void websrv_fl_add_xyplot_overlay( FL_OBJECT * ob,
                            int         id,
                            float     * x,
                            float     * y,
                            int         n,
                            FL_COLOR    col ){
FLI_XYPLOT_SPEC *spec = (FLI_XYPLOT_SPEC *)(ob->spec);

if (n>MAX_FLOAT_WEBSOCKMSG) {
   LOG_E(UTIL,"Buffer %i too small for %i iqs...\n",id,n);
   return;
}

  
  spec->n[id]=n; 
  spec->buff[id].buf[0].data_xy[0]=x[0];
  spec->buff[id].buf[0].data_xy[1]=y[0];
  for ( int i=1; i<n; i++) {
	if (x[i-1] <= x[i] ) {
	  spec->buff[id].buf[0].data_xy[2*i]=x[i];
	  spec->buff[id].buf[0].data_xy[(2*i)+1]=y[i];
	} else {
	  for (int j=0; j<i; j++) {
	     if (spec->buff[id].buf[0].data_xy[2*j]>x[i]) {
		   for (int k=i;k>=j;k--) {
	         spec->buff[id].buf[0].data_xy[2*(k+1)]=spec->buff[id].buf[0].data_xy[2*k];
	         spec->buff[id].buf[0].data_xy[(2*(k+1))+1]=spec->buff[id].buf[0].data_xy[(2*k)+1];
		   }
	       spec->buff[id].buf[0].data_xy[2*j]=x[i];
	       spec->buff[id].buf[0].data_xy[(2*j)+1]=y[i];
	    }
      }
	}
  }
};

void websrv_fl_set_xyplot_data( FL_OBJECT  * ob,
                         float      * x,
                         float      * y,
                         int          n,
                         const char * title,
                         const char * xlabel,
                         const char * ylabel ){
  websrv_fl_add_xyplot_overlay(ob,	0, x,y,n,	FL_YELLOWGREEN);				 
};


void websrv_fl_get_xyplot_data( FL_OBJECT * ob,
                         float     * x,
                         float     * y,
                         int       * n ){
FLI_XYPLOT_SPEC *spec = (FLI_XYPLOT_SPEC *)(ob->spec);
*n=spec->n[0];	
};

void websrv_fl_get_xyplot_data_pointer( FL_OBJECT  * ob,
                                 int          id,
                                 float     ** x,
                                 float     ** y,
                                 int        *n ){
FLI_XYPLOT_SPEC *spec = (FLI_XYPLOT_SPEC *)(ob->spec);
*x=spec->buff[0].buf[id].data_xy;
*y=spec->buff[0].buf[id].data_xy;
*n=spec->n[id];
};

/*----------------------------------------------------------------------*/
/* new functions for interfacing with webserver                          */

int websrv_nf_getdata(FL_OBJECT *graph, int layer, websrv_scopedata_msg_t **msg, int *nummsg) {
	FLI_XYPLOT_SPEC *spec = (FLI_XYPLOT_SPEC *)(graph->spec);
	*msg=spec->buff[layer].buf;
	*nummsg=(spec->n[layer]/MAX_FLOAT_WEBSOCKMSG);
	if ((spec->n[layer]%MAX_FLOAT_WEBSOCKMSG) != 0) (*nummsg)++;
	return spec->n[layer];
}



