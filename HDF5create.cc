/*****************************************************************************
*
* ALPS/HDF5test: HDF5 performance test
*
* Copyright (C) 2013 by Ryo IGARASHI <rigarash@issp.u-tokyo.ac.jp>
*
* This software is published under the ALPS Application License; you
* can use, redistribute it and/or modify it under the terms of the
* license, either version 1 or (at your option) any later version.
* 
* You should have received a copy of the ALPS Application License
* along with this software; see the file LICENSE. If not, the license
* is also available from http://alps.comp-phys.org/.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT 
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
* DEALINGS IN THE SOFTWARE.
*
*****************************************************************************/
#include <alps/alea/observable.h>
#include <alps/alea/observableset.h>
#include <alps/osiris.h>

#include <boost/timer/timer.hpp>
#include <boost/random.hpp>
#include <boost/filesystem.hpp>

#include <iostream>

static long const N = 1<<27;

int main(int argc, char** argv) {
    boost::timer::cpu_timer t;

    alps::ObservableSet obs;
    obs << alps::RealObservable("a");
    obs.reset(true);

    boost::mt19937 eng(0);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> >
        random_01(eng, boost::uniform_real<>());
    t.stop();

    t.start();
    for (long i = 0; i < N; ++i) {
        obs["a"] << random_01();
    }
    t.stop();
    std::cout << "Observable storing:" << t.format(6);

    // Assuming that the pre-stored variable is inside source directory.
    boost::filesystem::path xdr("test.xdr");
    alps::OXDRFileDump odp(xdr);
    t.start();
    obs.save(odp);
    t.stop();
    std::cout << "XDR save:          " << t.format(6);

    return 0;
}
