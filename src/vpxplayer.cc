#include "includes.h"
#include "vpxplayer.h"
#include "globals.h"

#define VPX_CODEC_DISABLE_COMPAT 1
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"
// vpx_codec_vp8_dx()

class VPXcodecData
{
public:
	vpx_codec_ctx_t  codec;
	int flags;
	vpx_codec_iter_t  iter;
    vpx_image_t      *img;
};

int vpxplayer::decoder_init()
{
	int res;
	vpxc=new VPXcodecData();
	vpxc->flags=0;
	res=vpx_codec_dec_init(&vpxc->codec,vpx_codec_vp8_dx(),NULL,vpxc->flags);
	if (res)
	{
		INFO(VPXDEC,"Failed to initialize vpx decoder\n");
		return 0;
	}
	vpxc->img=NULL;
	return 1;
}

void vpxplayer::decode_frame(int &frameno, uint8_t *pixels, int width, int height)
{
	int i,j;
	INFO(VPXDEC,"VPXdec ");

	if (vpxc->img)
		vpxc->img=vpx_codec_get_frame(&vpxc->codec, &vpxc->iter);

	if (!vpxc->img)
	{
		int framesize=read_frame(frameno);
		INFO(VPXDEC,"len=%-6d ",framesize);
		for (i=0; i<12; i++) INFO(VPXDEC,"%02x%s",frame[i],(i%4==3)?" ":"");
		if(vpx_codec_decode(&vpxc->codec, buffer, framesize, NULL, 0))
		{
			INFO(VPXDEC,"VPX: Decode failed\n");
		}
		vpxc->iter=NULL;
	} else {
		INFO(VPXDEC,"VPX: already got frame\n");
	}
	vpxc->img=vpx_codec_get_frame(&vpxc->codec, &vpxc->iter);
	if (!vpxc->img)
	{
		INFO(VPXDEC,"VPX: returned with no data\n");
		return;
	}
	unsigned char *buf =vpxc->img->planes[0];
	int src_height=vpxc->img->d_h;
	int src_width=vpxc->img->d_w;
	int stride=vpxc->img->stride[0];
	for (i=0; i<src_height; i++)
	{
		if (i>=height) break;
		for (j=0; j<src_width; j++)
		{
			if (j>=width) break;
			// very inefficient, but ok for now
			int y,cb,cr,r,g,b;
			y=vpxc->img->planes[0][i*stride+j];
			cb=vpxc->img->planes[1][i/2*stride/2+j/2]-128;
			cr=vpxc->img->planes[2][i/2*stride/2+j/2]-128;
			r = y + ((359 * cr) >> 8);
			g = y - ((11 * cb) >> 5) - ((183 * cr) >> 8);
			b = y + ((227 * cb) >> 7);
			if (r<0) r=0; if (r>255) r=255;
			if (g<0) g=0; if (g>255) g=255;
			if (b<0) b=0; if (b>255) b=255;
			pixels[i*width*3+j*3]=r;
			pixels[i*width*3+j*3+1]=g;
			pixels[i*width*3+j*3+2]=b;
		}
	}
	INFO(VPXDEC,"frame ok\n");
}

void vpxplayer::decoder_done()
{
	if (decodeddata)
	{
		free(decodeddata);
		decodeddata=NULL;
	}
	if (vpxc)
	{
		delete vpxc;
		vpxc=NULL;
	}
}

int vpxplayer::load_next_frame(int gltexture, uint64_t time, int flags)
{
	int i;
	if (!decodeddata)
	{
		glbufheight=16;
		glbufwidth=16;
		while (glbufheight<height && glbufheight<maxTexSize)
			glbufheight*=2;
		while (glbufwidth<width && glbufwidth<maxTexSize)
			glbufwidth*=2;
		decodeddata=(uint8_t*) malloc(glbufheight*glbufheight*3);
		INFO(VPXDEC,"VPX Allocated decoded frame buffer: %dx%d\n",glbufwidth,glbufheight);
	}
	uint64_t filmtime=uspf*vidx.size();
	time%=filmtime;
	DBG(VPXDEC,"time:%d last:%d uspf:%d eframe:%d frame:%d\n",time,lastframeadvance,uspf,time/uspf,crtframe);
	if (lastframeadvance>time) lastframeadvance=0;
	// crtframe is a long way into the future (2s) and time is low (1s)
	// thus we can reset to the beginning of the movie
	// this will happen when entering a menu
	if (time<1000000 && crtframe*uspf>time+2000000) crtframe=0;
	if (!flags) if (time/uspf<crtframe || lastframeadvance+uspf/2>time) return 0;
	lastframeadvance=time;
	decode_frame(crtframe,decodeddata,glbufwidth,glbufheight);
	// bind the texture
	glBindTexture(GL_TEXTURE_2D, gltexture);
//	DBG(VPXDEC,"Binding to texture %d\n",gltexture);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage2D(GL_TEXTURE_2D, 0, 3, glbufwidth, glbufheight, 0, GL_RGB, GL_UNSIGNED_BYTE, decodeddata);
//	for (i=0; i<glbufwidth*glbufheight*3; i+=1237) DBG(VPXDEC,"%d ",decodeddata[i]);
}
