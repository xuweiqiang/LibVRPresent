
/* 
The sample code for using the "DSub"

Note:This file just give you a refrence for using the "DSub" process the subtitle stream.
You can't compile it, because it depended on other files, and also, some APIs you need't care,
just attend the APIs named like "ALI_DTV_SUB_XXX_XXX", these APIs are the "DSub" interface,
all needed comments are added in the file.
Please reading the document "ALI DTV SUBTITLE SDK DEVELOPER¡¯S GUIDE.doc" firstly,
which file will give you detailed information about the "DSub".   

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The head files must included in your program, pls note the sequence.
#include "ALI_DTV_SUB_typedef.h"  
#include "ALI_DTV_SUB_api.h"

#define USE_SUB_SYNC  //for sub sync, pls implement by yourself, the "DSub" not have this function
#define TSSIZE 188

typedef struct  {
    uint8_t *buffer;
    int length;
    int pos;
} tPESPkt;   //pes struct I defined, you can use others 


typedef struct  {
    uint32_t *buffer;
    int width;
    int height;
    uint32_t colormap[256];
} tDrawCtxt;  // for sub page rendering


#define STD_MIN(a,b)  (((a)<(b)) ? (a) : (b))
#define STD_MAX(a,b)  (((a)>(b)) ? (a) : (b))


#ifdef USE_SUB_SYNC
#include "sync.h"    // for time synchronization, implement by yourself 
#include "ivi_pc.h"  // to control data producing and consuming, implement by yourself   
#endif

#if  FLTK_SHOW      // fltk lib for window management and osd painting, implement by yourself
#include <fltk/run.h>
#include <fltk/Window.h>
#include <fltk/ValueSlider.h>
#include <fltk/draw.h>
#include <fltk/RadioButton.h>
#include <fltk/rgbImage.h>
#endif



typedef struct{
unsigned int sub_pid;	  //the sub pid
tPESPkt pespacket; //the pes packet buf
HANDLE ali_dtv_sub;   //the handle which created by "DSub"

// the following parameters of this structure just for demo needed, implement by yourself    
HANDLE	  sub_thread; 
Window * sub_window;
#ifdef USE_SUB_SYNC
HANDLE	  sub_render_thread;
CSynchronizer* sync;
HANDLE sub_pc;
#endif
bool  sub_dec_start;
bool  sub_win_start;
bool  sub_en;

}tDvbSub,*pDvbSub;

// you don't need care this definition, it just for demo synchronization
typedef struct
{
void *ds;
UINT64 pts;
}my_ds;


extern "C"{
HANDLE CreateDvbSubResource(int sub_pid,int page_id,int ancillary_page_id);
int DeleteDvbSubResource(HANDLE sub);	
int Dvbsub_DataPush(void* priv, unsigned char *data, unsigned int size);  
}


static tDvbSub dvbsub;   


//YUV convert to RGB
void YCbCrT_to_RGBA(int Y, int Cb, int Cr, int T,
                    uint8_t *R, uint8_t *G, uint8_t *B, uint8_t *A) {
    int r, g, b;
    int Ey, Epb, Epr;
    Ey = (Y - 16);
    Epb = (Cb - 128);
    Epr = (Cr - 128);
    /* ITU-R 709 */
    r = STD_MAX(STD_MIN(((298 * Ey             + 460 * Epr) / 256), 255), 0);
    g = STD_MAX(STD_MIN(((298 * Ey -  55 * Epb - 137 * Epr) / 256), 255), 0);
    b = STD_MAX(STD_MIN(((298 * Ey + 543 * Epb            ) / 256), 255), 0);

    *R = (uint8_t)r;
    *G = (uint8_t)g;
    *B = (uint8_t)b;
    *A = T;
}


// The callback function you must implement by yourself, this routine will be called when page rendering
void DrawHLineCB(void *priv, int x, int y, int length, int thickness,
               uint8_t pseudo_color) {
    tDrawCtxt* dc = (tDrawCtxt*)priv;
    uint32_t color = dc->colormap[pseudo_color];
    if (thickness == 2)
        for (; length > 0; --length, ++x) {
            dc->buffer[y*dc->width + x] = color;
            dc->buffer[(y+1)*dc->width + x] = color;
        }
    else
        for (; length > 0; --length, ++x)
            dc->buffer[y*dc->width + x] = color;
}

// The callback function you must implement by yourself, this routine will be called by "DSub" when page rendering
void ColorSetCB(void *priv, uint8_t pseudo_color, uint8_t Y,
               uint8_t Cb, uint8_t Cr, uint8_t T) {
    tDrawCtxt* dc = (tDrawCtxt*)priv;
    uint8_t r, g, b, a;
    uint32_t rgba;
    YCbCrT_to_RGBA(Y, Cb, Cr, T, &r, &g, &b, &a);
    rgba = (a << 24) | (b << 16) | (g << 8) | r;
//    printf("abgr: 0x%x\n",rgba);
    dc->colormap[pseudo_color] = rgba;
}

// The callback function you must implement by yourself, this routine will be called when ds(means a subtitle page) is ready
// pls save the ds and it's pts to your ds list, render it according to it's pts and sync with video and audio
void display_sets_recv_callback(void *priv, void* ds,UINT64 pts){
    pDvbSub dvbsub;
    dvbsub=(pDvbSub)priv;
	  my_ds* myds=NULL;
    if(ds){
	    myds=(my_ds*)malloc(sizeof(my_ds)); 
		if(myds){
          myds->ds=ds;
          myds->pts=pts;
		  if(ivi_pc_produce(dvbsub->sub_pc, myds)==-1){
			ALI_DTV_SUB_Display_Set_Destroy(dvbsub->ali_dtv_sub,ds);
			free(myds);
		  }
		}
		else{
			ALI_DTV_SUB_Display_Set_Destroy(dvbsub->ali_dtv_sub,ds);
		}
	}
}

// get the ds from my ds list, you needn't care this function
static void dvb_sub_fetch_ds(void *parm1, void *ds, void *data)
{
     pDvbSub dvbsub=(pDvbSub)parm1;
     *((int *)ds)=(int)data;
}


// a sample for rendering subtitle page, pls attend the function like "ALI_DTV_SUB_XXX_XXX", the detailed description
// see the sdk developer guide. 
unsigned int __stdcall dvb_sub_render_thread(void *arg)
{
    pDvbSub dvbsub=(pDvbSub)arg;
    int i;
	tDrawCtxt *dc=NULL;
	my_ds* myds=NULL;
	int number_of_regions=0;
    while((ivi_pc_how_many_prod_item(dvbsub->sub_pc)<3)&& dvbsub->sub_en){
        Sleep(1);
    }	
	if(!dvbsub->sub_en){
        return 0;
	}
    dc = (tDrawCtxt *)malloc(sizeof(tDrawCtxt));
    if (dc == NULL || dvbsub->sync==NULL || dvbsub->sub_pc==NULL){
        dvbsub->sub_en=0;
        return 0;   
    }
   while (dvbsub->sub_en==1 || (dvbsub->sub_en==0 && (ivi_pc_how_many_prod_item(dvbsub->sub_pc)!=0)))  { 
        int ds_t=0;
        if(ivi_pc_consume(dvbsub->sub_pc,dvb_sub_fetch_ds,(void *)dvbsub,(void *)&ds_t)==-1)
		   break;
        if(ds_t==0){
           continue;
        }
        else{
           myds=(my_ds*)ds_t;
		   if(dvbsub->sub_en==0){
		   	 printf("sub_en==0, discard this DS!!!\n");
	         ALI_DTV_SUB_Display_Set_Destroy(dvbsub->ali_dtv_sub,myds->ds);
			 free(myds);
			 continue;
		   }
        }        
		number_of_regions=ALI_DTV_SUB_Get_Region_Count_Of_DS(dvbsub->ali_dtv_sub,myds->ds);
        printf("ds =%p ,Number_of_region_compositions = %d\n",myds->ds,number_of_regions);
        if(number_of_regions==0){
 	         ALI_DTV_SUB_Display_Set_Destroy(dvbsub->ali_dtv_sub,myds->ds);
			 free(myds);       
            continue;
        }
        else{
     //    sleep(1);
            if(myds->pts==0){
              Sleep(500);  
			}
			else{
              CSynchronizer_sync(dvbsub->sync,myds->pts);    
		    }
        }
#if 1       
    	setcolor(GRAY66);
	    fillrect(0,0,dvbsub->sub_window->w(),dvbsub->sub_window->h());
        for (i = 0; i < number_of_regions; ++i) {
            region_info rg;
            void* cntx;
            int color, y, j;            
            if (ALI_DTV_SUB_Get_Region_Info(dvbsub->ali_dtv_sub,myds->ds,&rg,i)== -1)
                continue;

            dc->width = rg.region_width;
            dc->height = rg.region_height;
            dc->buffer = (uint32_t *)malloc(sizeof(uint32_t)* dc->width * dc->height);
            if (dc->buffer == NULL) {
                continue;
            }
            /* Create rendering context */
            cntx =ALI_DTV_SUB_Region_Render_Context_Create(dvbsub->ali_dtv_sub,myds->ds,rg.region_id,
                                                   DrawHLineCB, NULL,
                                                   ColorSetCB, dc);
            if (cntx == 0) {
                free(dc->buffer);
                continue;
            }
            color = rg.bkcolor;
            if(color!=-1){
               if (color < 0 && color > 255)
                 color = 0;
               for (y = 0; y < dc->height; ++y)
                 DrawHLineCB(dc, 0, y, dc->width, 1, color);
               printf("Regin BG colorIdx=%d\n",color);
            }
            else{
               printf("No Regin BG color\n");
            }
            /* Do actual rendering */
            ALI_DTV_SUB_Region_Actual_Rendering(dvbsub->ali_dtv_sub,cntx);
                printf("Draw ........\n");
                printf("  PTS=%llu\n",myds->pts);
                printf("  region id=%d\n", rg.region_id);
				printf("  position=(%d : %d)\n   size=(%d : %d)\n",rg.region_horizontal_address, rg.region_vertical_address, rg.region_width,rg.region_height);
    		drawimage((uchar*)dc->buffer,RGB32 ,
        		fltk::Rectangle(rg.region_horizontal_address, rg.region_vertical_address-200, rg.region_width, rg.region_height));        

            ALI_DTV_SUB_Region_Render_Context_Destroy(dvbsub->ali_dtv_sub,cntx);
            free(dc->buffer);
        }
#endif        
	     ALI_DTV_SUB_Display_Set_Destroy(dvbsub->ali_dtv_sub,myds->ds);
		 free(myds);
		 myds=NULL;
  }
   free(dc);
   return 0;
}


uint16_t get_pid(const uint8_t *tspacket) {
    return (((uint16_t)tspacket[1] & 0x1f) << 8) + tspacket[2];
}


int get_pes_packet_length(const tPESPkt *packet) {
    return ((uint16_t)packet->buffer[4] << 8) + packet->buffer[5] + 6;
}


// a routine for rebuilding  the PES packet from the ts packet, 
// maybe you need implement by yourself, because this routine is not very perfect(such as not ts packets continuity detect)  
static int nPesStartFlag = 0;
int build_pes_pkt(tPESPkt *pespacket, const uint8_t *tspacket) {
    int pesstart = 4;
    int pes_len=0;

    if ((tspacket[3] & 0x10) == 0) /* has payload? */
        return 0;
	if (nPesStartFlag==0)
	{
		// the payload_unit_start_indicator ,use it to confirm whether have PES start in TS;
		if (0x40==(tspacket[1]&0x40) )  
			nPesStartFlag = 1;
		else
			return 0;
	}  

    if (pespacket->buffer == NULL ||
        pespacket->pos + TSSIZE > pespacket->length) {
        pespacket->length = 2*pespacket->length + 1000;
        pespacket->buffer = (uint8_t *)realloc(pespacket->buffer, pespacket->length);
    }
    if (tspacket[3] & 0x20) { /* adaptation field? */
       if(tspacket[4]>=183){// allow 0 - 182
        printf("SUB  illegal payload length\n");
        pes_len=get_pes_packet_length(pespacket);
        nPesStartFlag = 0;
        pespacket->pos=0;
		    return 0;
	    }
       pesstart += 1 + tspacket[4];
    }
    memcpy(pespacket->buffer + pespacket->pos, tspacket + pesstart,
           TSSIZE - pesstart);
    pespacket->pos += TSSIZE - pesstart;

    if(pespacket->pos <= 5 ||
        pespacket->pos < get_pes_packet_length(pespacket))
        return 0;
	  nPesStartFlag = 0;
    return 1;
}


// a routine for recv subtitle ts packets and rebuiled PES, then push PES to "DSub" for parsing 
int Dvbsub_DataPush(void* priv, unsigned char *data, unsigned int size)
{
   pDvbSub dvbsub=(pDvbSub)priv;
   uint8_t *buf_begin=data;
    int remainsize;
     if(size>= TSSIZE){    
        if (get_pid(buf_begin) != dvbsub->sub_pid){
              return -1;
        }
        if (build_pes_pkt(&(dvbsub->pespacket), buf_begin)){
            int length;

            /* Send PES packet for subtitle decoder to handle */
            if(ALI_DTV_SUB_Push_PES_Pkt(dvbsub->ali_dtv_sub,(char *)dvbsub->pespacket.buffer)!=0){
				      dvbsub->pespacket.pos=0; //rest pes buf;
			      }
			      else{
              length = get_pes_packet_length(&dvbsub->pespacket);
              memcpy(dvbsub->pespacket.buffer, dvbsub->pespacket.buffer + length,dvbsub->pespacket.pos - length);
              dvbsub->pespacket.pos -= length;
		        }
        }
    }
   return 0;
}


// a funtion about the subtitle window, you needn't care this
unsigned int __stdcall dvb_sub_window_thread(void *arg)
{
   pDvbSub dvbsub=(pDvbSub)arg;
#if 1
   if(dvbsub->sub_window = new Window(720,576,"DVB SUB TEST")){ // create window
	dvbsub->sub_window->begin();
	dvbsub->sub_window->end();
	dvbsub->sub_window->show(0,NULL);
	dvbsub->sub_win_start=1;
	run();   
   }
#endif
   dvbsub->sub_win_start=0;
   return 0;
}


// a sample function for initializing the "DSub", pls attend the function like "ALI_DTV_SUB_XXX_XXX", the detailed description
// see the sdk developer guide.
HANDLE CreateDvbSubResource(int sub_pid,int page_id,int ancillary_page_id)
{
    dvbsub.sub_pid = sub_pid;
    dvbsub.sub_thread=0;
#ifdef USE_SUB_SYNC
    dvbsub.sub_render_thread=0;
    dvbsub.sync=GetSynchronizer();
    dvbsub.sub_pc=ivi_pc_creat(10, 0);
    if(dvbsub.sub_pc==NULL){
        DestroySynchronizer(dvbsub.sync);
    }
#endif    
    dvbsub.sub_en=1;
    dvbsub.sub_dec_start=0;
	  dvbsub.sub_win_start=0;
    dvbsub.pespacket.buffer = NULL;
    dvbsub.pespacket.length = 0;
    dvbsub.pespacket.pos = 0;
	  dvbsub.sub_window=NULL;
    if(!(dvbsub.ali_dtv_sub=ALI_DTV_SUB_Create(page_id,ancillary_page_id, display_sets_recv_callback,(void *)&dvbsub))) {
         return 0;
    }
	 if(NULL==(dvbsub.sub_thread=(HANDLE)_beginthreadex(NULL,0,dvb_sub_window_thread,(void *)&dvbsub, 0,NULL)))
	{
			printf("Can't create SUB win thread!");
			goto close_sub ;
	}	 
	  Sleep(500);
	  setcolor(GRAY66);
    fillrect(0,0,dvbsub.sub_window->w(),dvbsub.sub_window->h());
    if(NULL==(dvbsub.sub_render_thread=(HANDLE)_beginthreadex(NULL,0,dvb_sub_render_thread,(void *)&dvbsub, 0,NULL)))
    {
    	printf("Can't create SUB render thread!");
    	goto close_sub ;
    }   
    return ((HANDLE)&dvbsub);
	
close_sub:
    
  if(dvbsub.sub_pc){ 
    ivi_pc_pause_wkup_cum(dvbsub.sub_pc);
    ivi_pc_pause_wkup_prod(dvbsub.sub_pc);
    }
   if(dvbsub.sub_window){		
	    if(dvbsub.sub_thread){
		   dvbsub.sub_window->hide();
		   WaitForSingleObject(dvbsub.sub_thread, INFINITE);
		   CloseHandle(dvbsub.sub_thread);
		   dvbsub.sub_thread = NULL;
	    }
		delete dvbsub.sub_window;
		dvbsub.sub_window=NULL;
    }
  if(dvbsub.sub_render_thread){
//     pthread_join(dvbsub.sub_render_thread,NULL);
    	WaitForSingleObject(dvbsub.sub_render_thread, INFINITE);
    	CloseHandle(dvbsub.sub_render_thread);
    	dvbsub.sub_render_thread = NULL;
	}
    DestroySynchronizer(dvbsub.sync);
    if(dvbsub.sub_pc){
       ivi_pc_close(dvbsub.sub_pc);
	   dvbsub.sub_pc=NULL;
    }
    if(dvbsub.ali_dtv_sub){
      ALI_DTV_SUB_Release(dvbsub.ali_dtv_sub);
	}
	if(dvbsub.pespacket.buffer)
      free(dvbsub.pespacket.buffer);
    return NULL;
}

// a sample for close the "DSub", pls attend the function like "ALI_DTV_SUB_XXX_XXX", the detailed description
// see the sdk developer guide.
int DeleteDvbSubResource(HANDLE sub)	
{ 
  if(sub){
     pDvbSub handle=(pDvbSub)sub;
	 if(handle==&dvbsub){
		  if(dvbsub.sub_pc){ 
		  	 dvbsub.sub_en=0;
			 Sleep(10);
			 ivi_pc_pause_wkup_prod(dvbsub.sub_pc);
			  if(ivi_pc_how_many_prod_item(dvbsub.sub_pc)==0){			 	
			   ivi_pc_pause_wkup_cum(dvbsub.sub_pc);
			  }
			 }
		   if(dvbsub.sub_render_thread){
		 		 WaitForSingleObject(dvbsub.sub_render_thread, INFINITE);
				 CloseHandle(dvbsub.sub_render_thread);
				 dvbsub.sub_render_thread = NULL;
			 }		  
			if(dvbsub.sub_window){		 
				 if(dvbsub.sub_thread){
					dvbsub.sub_window->hide();
					printf("\n ===== Wait Sub Window Close ===== \n");
					WaitForSingleObject(dvbsub.sub_thread, INFINITE);					
					CloseHandle(dvbsub.sub_thread);
					dvbsub.sub_thread = NULL;
				 }
				 delete dvbsub.sub_window;
				 dvbsub.sub_window=NULL;
			 }
			 DestroySynchronizer(dvbsub.sync);
			 if(dvbsub.sub_pc){
				ivi_pc_close(dvbsub.sub_pc);
				dvbsub.sub_pc=NULL;
			 }
			 if(dvbsub.ali_dtv_sub){
			   ALI_DTV_SUB_Release(dvbsub.ali_dtv_sub);
			 }
			 if(dvbsub.pespacket.buffer)
			   free(dvbsub.pespacket.buffer);
      return 0;
	 }
    }
   return -1;
}



