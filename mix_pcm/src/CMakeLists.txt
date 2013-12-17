SET(LIB_SRC speex_decode.cpp speex_encode.cpp AmMultiPartyMixer.cpp)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/lib/)

ADD_LIBRARY(mix_pcm STATIC ${LIB_SRC})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/output/include/)

INSTALL(FILES mix_pcm.h yyt_type.h  speex_decode.h speex_encode.h AmMultiPartyMixer.h SampleArray.h SampleArray.cc  DESTINATION ${PROJECT_SOURCE_DIR}/output/include)
INSTALL(TARGETS mix_pcm ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/output/lib/)

SET(CMAKE_BUILD_TYPE "Debug") 
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
