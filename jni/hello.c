#include <stdio.h>
#include "cn_itcast_lame_MainActivity.h"
#include <lame.h>
#include <android/log.h>
#define LOG_TAG "System.out"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

char * Jstring2CStr(JNIEnv * env, jstring str){
	char* rtn = NULL;
	jclass classString = (*env)->FindClass(env, "java/lang/String");
	jmethodID mid = (*env)->GetMethodID(env, classString, "getBytes", "()[B");
	jbyteArray barray = (*env)->CallObjectMethod(env, str, mid);
	int len = (*env)->GetArrayLength(env, barray);
	jbyte* ba = (*env)->GetByteArrayElements(env, barray, 0);
	if(len > 0){
		rtn = (char*)malloc(len + 1);
		memcpy(rtn, ba, len);
		rtn[len] = 0;
	}
	(*env)->ReleaseByteArrayElements(env, barray, ba, 0);
	return rtn;
}


/*
 * as above, but input has L & R channel data interleaved.
 * NOTE:
 * num_samples = number of samples in the L (or R)
 * channel, not the total number of samples in pcm[]
 */
/*int CDECL lame_encode_buffer_interleaved(*/
 /*       lame_global_flags*  gfp,            global context handlei        */
 /*       short int           pcm[],          PCM data for left and right
                                              channel, interleaved          */
/*        int                 num_samples,    number of samples per channel,
                                              _not_ number of samples in
                                              pcm[]                         */
 /*        unsigned char*      mp3buf,        pointer to encoded MP3 stream */
 /*        int                 mp3buf_size );number of valid octets in this
                                              stream                        */
JNIEXPORT void JNICALL Java_cn_itcast_lame_MainActivity_convertToMp3
  (JNIEnv * env, jobject obj, jstring jwav, jstring jmp3){

	char* wav = Jstring2CStr(env, jwav);
	char* mp3 = Jstring2CStr(env, jmp3);
	LOGI("wav = %s", wav);
	LOGI("mp3 = %s", mp3);

	FILE* fwav;
	if((fwav = fopen(wav, "rb")) == NULL){
		LOGI("not found wav file!");
		return ;
	}
	FILE* fmp3;
	if((fmp3 = fopen(mp3, "wb")) == NULL){
			LOGI("not found mp3 file!");
			return;
		}

	short int pcm[8192*2];
	unsigned char mp3buf[8192];

	//初始化lame
	lame_t lame = lame_init();
	//设置采样率
	lame_set_in_samplerate(lame, 44100);
	//设置声道
	lame_set_num_channels(lame, 2);
	//设置编码方式
	lame_set_VBR(lame, vbr_default);
	//初始化参数
	lame_init_params(lame);

	//读写文件
	int read = 0;
	int write = 0;
	int total = 0;
	while((read = fread(pcm, sizeof(short int)*2, 8192, fwav)) != 0){	//读

		write = lame_encode_buffer_interleaved(lame, pcm, read, mp3buf, 8192);	//转
		total += write;
		LOGI("converting................%d", total);
		fwrite(mp3buf, sizeof(unsigned char), write, fmp3);				//写
	}

	if(read == 0){
		lame_encode_flush(lame, mp3buf, 8192);
	}
	lame_close(lame);
	fclose(fwav);
	fclose(fmp3);

	LOGI("convert finish");
}

JNIEXPORT jstring JNICALL Java_cn_itcast_lame_MainActivity_getVersion
  (JNIEnv * env, jobject obj){

	return (*env)->NewStringUTF(env, get_lame_version());
}
