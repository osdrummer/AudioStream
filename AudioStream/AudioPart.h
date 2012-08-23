//
//  AudioPart.h
//  AudioStream
//
//  Created by Michael Kolesov on 7/30/12.
//  Copyright (c) 2012 Michael Kolesov. All rights reserved.
//  Based on Apple code example. Copyright (c) 2007 Apple Inc. All Rights Reserved.  

#ifndef AudioStream_AudioPart_h
#define AudioStream_AudioPart_h

#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <AudioToolbox/AudioToolbox.h>

#define PRINTERROR(LABEL)	printf("%s err %4.4s %ld\n", LABEL, (char*)&err, err)

#define kNumAQBufs  4			// number of audio queue buffers we allocate
#define kAQMaxPacketDescs 512		// number of packet descriptions in our array
#define kPreStreamedBufs  3         // minimum recommended pre-streamed buffers


struct MyData
{
	AudioFileStreamID audioFileStream;	// the audio file stream parser
    
	AudioQueueRef audioQueue;								// the audio queue
	AudioQueueBufferRef audioQueueBuffer[kNumAQBufs];		// audio queue buffers
	
	AudioStreamPacketDescription packetDescs[kAQMaxPacketDescs];	// packet descriptions for enqueuing audio
	
    int preStreamedBuffers;         // number of buffers downloaded before start playing
    int bitRate;                    // stream bitrate in Kbps
    int bufSize;                    // size of audio buffer
    
	unsigned int fillBufferIndex;	// the index of the audioQueueBuffer that is being filled
	size_t bytesFilled;				// how many bytes have been filled
	size_t packetsFilled;			// how many packets have been filled
    
	bool inuse[kNumAQBufs];			// flags to indicate that a buffer is still in use
	bool started;					// flag to indicate that the queue has been started
	bool failed;					// flag to indicate an error occurred
    
    
	pthread_mutex_t mutex;			// a mutex to protect the inuse flags
	pthread_cond_t cond;			// a condition varable for handling the inuse flags
	pthread_cond_t done;			// a condition varable for handling the inuse flags
    bool finishing;                 // flag to finish audio data processing
    bool finishingReady;            // flag to show finishing is ready
};
typedef struct MyData MyData;


void MyAudioQueueOutputCallback(void* inClientData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer);
void MyAudioQueueIsRunningCallback(void *inUserData, AudioQueueRef inAQ, AudioQueuePropertyID inID);

void MyPropertyListenerProc( void *							inClientData,
                            AudioFileStreamID				inAudioFileStream,
                            AudioFileStreamPropertyID		inPropertyID,
                            UInt32 *						ioFlags);

void MyPacketsProc(	void *						inClientData,
                   UInt32						inNumberBytes,
                   UInt32						inNumberPackets,
                   const void *					inInputData,
                   AudioStreamPacketDescription	*inPacketDescriptions);

OSStatus MyEnqueueBuffer(MyData* myData);
void WaitForFreeBuffer(MyData* myData);

int AudioPartInit();
int AudioPartParser( const void * buf, ssize_t bytesRecvd );
int AudioPartFinish( bool immediate );
int AudioPartNewStream ( AudioFileTypeID inStreamTypeHint, int bitRate );


#endif
