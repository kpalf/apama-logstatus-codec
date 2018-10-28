# apama-logstatus-plugin
Apama connectivity codec for parsing correlator log lines


## Supported Apama version

This works with Apama 10.3.0.1 or later

## Building the plugin

In an Apama command prompt on Linux run:

    mkdir -p $APAMA_WORK/lib 
    g++ -std=c++11 --shared -fPIC -o $APAMA_WORK/lib/libLogStatusCodec.so ../LogStatusCodec.cpp -I$APAMA_HOME/include -lapclient -L$APAMA_HOME/lib

On Windows run:

    g++ -std=c++11 --shared -o %APAMA_WORK%\lib\LogStatusCodec.dll ..\LogStatusCodec.cpp -I%APAMA_HOME%/include -lapclient -L%APAMA_HOME%/lib

## Running tests

To run the tests for the plugin you will need to use an Apama command prompt to run the tests from within the tests directory:

    pysys run



