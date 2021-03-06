// Copyright 2004-present Facebook. All Rights Reserved.

#import <Foundation/Foundation.h>

#import <FBControlCore/FBFileConsumer.h>
#import <FBControlCore/FBFuture.h>

NS_ASSUME_NONNULL_BEGIN

/**
 The File Reader States
 */
typedef NS_ENUM(NSUInteger, FBFileReaderState) {
  FBFileReaderStateNotStarted = 0,
  FBFileReaderStateReading = 1,
  FBFileReaderStateTerminating = 2,
  FBFileReaderStateTerminatedNormally = 3,
  FBFileReaderStateTerminatedAbnormally = 4,
};

/**
 Reads a file in the background, forwarding to a consumer.
 you need to call startReading to setup the channel and stopReading when
 you finish consumption. not calling stopReading will lead to undefined behaviour
 and possibly crashes.
 */
@interface FBFileReader : NSObject

#pragma mark Initializers

/**
 Creates a File Reader from a File Handle.

 @param fileHandle the file handle to read from. It will be closed when the reader stops.
 @param consumer the consumer to forward to.
 @param logger the logger to use.
 @return a File Reader.
 */
+ (instancetype)readerWithFileHandle:(NSFileHandle *)fileHandle consumer:(id<FBFileConsumer>)consumer logger:(nullable id<FBControlCoreLogger>)logger;

/**
 Creates a File Reader from a File Handle.

 @param fileHandle the file handle to read from. It will be closed when the reader stops.
 @param consumer the consumer to forward to.
 @return a File Reader.
 */
+ (instancetype)readerWithFileHandle:(NSFileHandle *)fileHandle consumer:(id<FBFileConsumer>)consumer;

/**
 Creates a File Reader for a File at Path.

 @param filePath the File Path to read from.
 @param consumer the consumer to forward to.
 @param logger the logger to use.
 @return a File Reader, that is available when the underlying file handle has been opened.
 */
+ (FBFuture<FBFileReader *> *)readerWithFilePath:(NSString *)filePath consumer:(id<FBFileConsumer>)consumer logger:(nullable id<FBControlCoreLogger>)logger;

/**
 Creates a File Reader for a File at Path.

 @param filePath the File Path to read from.
 @param consumer the consumer to forward to.
 @return a File Reader, that is available when the underlying file handle has been opened.
 */
+ (FBFuture<FBFileReader *> *)readerWithFilePath:(NSString *)filePath consumer:(id<FBFileConsumer>)consumer;

#pragma mark Public Methods

/**
 Starts the Consumption of the File.

 @return a Future that resolves when the channel is setup.
 */
- (FBFuture<NSNull *> *)startReading;

/**
 Stops Reading the file.

 @return a Future that resolves when the consumption of the file has finished. The wrapped NSNumber is the FBFileReaderState of termination.
 */
- (FBFuture<NSNumber *> *)stopReading;

#pragma mark Properties

/**
 The current state of the file reader
 */
@property (atomic, assign, readonly) FBFileReaderState state;

/**
 A Future that resolves when the reading has ended. The wrapped NSNumber is the FBFileReaderState of termination.
 */
@property (nonatomic, strong, readonly) FBFuture<NSNull *> *completed;

@end

NS_ASSUME_NONNULL_END
