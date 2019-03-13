/*
 * Copyright (c) 2018 - Christian Berger <christian.berger@gu.se>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

// Include the single-file, header-only cluon library.
#include "cluon-complete.hpp"

// Include the message specification.
#include "opendlv-standard-message-set.hpp"

int32_t main(int32_t argc, char **argv) {
    // Parse the command line parameters to read the CID number that specifies an OD4Session to exchange data.
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if (0 == commandlineArguments.count("cid")) {
        std::cerr << argv[0] << " demonstrates a small ping-pong application between C++ and JavaScript." << std::endl;
        std::cerr << "Usage:   " << argv[0] << " --cid=<OpenDaVINCI session>" << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=111" << std::endl;
        return 1;
    }
    else {
        // Interface to a running OpenDaVINCI session 
        cluon::OD4Session od4{static_cast<uint16_t>(std::stoi(commandlineArguments["cid"]))};
        // Define a callback lambda...
        auto onPedalPositionReading = [](cluon::data::Envelope &&env){
            opendlv::proxy::PedalPositionReading p = cluon::extractMessage<opendlv::proxy::PedalPositionReading>(std::move(env));
            if(env.senderStamp() == 1901){
                std::cout << "Brake Position: " << p.position()<< std::endl;
            }else{
                std::cout <<env.senderStamp()<< "?? Position: " << p.position()<< std::endl;

            }
        };
        od4.dataTrigger(opendlv::proxy::PedalPositionReading::ID(), onPedalPositionReading);

        
        // Now, we define a time-triggered sending lambda.
        uint32_t counter{0};
        auto ping = [&od4, &counter](){
            // Define a message...
            
            return true;
        };
        // Finally, register the lambda as time-triggered function.
        const float RUN_AT_TWO_HERTZ{2.0f};
        od4.timeTrigger(RUN_AT_TWO_HERTZ, ping); // Won't return until stopped.
        }

    return 0;
}
