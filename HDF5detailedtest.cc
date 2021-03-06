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
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <cstddef>
#include <vector>

#include <fj_tool/fipp.h>

static std::size_t const N = 200;
static std::size_t const SIZE = 1<<16;

int main(int argc, char** argv) {
    boost::timer::cpu_timer t;

    alps::ObservableSet obs;
    obs << alps::RealObservable("a");
    obs.reset(true);

    // Assuming that the dump is located in current directory.
    boost::filesystem::path dump("test.xdr");
    alps::IXDRFileDump idp(dump);
    t.start();
        obs.load(idp);
    t.stop();
    std::cout << obs;
    std::cout << "XDR load:          " << t.format(6) << std::flush;

    // prepare string of i
    std::vector<std::string> iv(N);
    for (std::size_t i = 0; i < N; ++i) {
        iv[i] = boost::lexical_cast<std::string>(i);
    }

    {
    boost::filesystem::path hdf5(boost::filesystem::unique_path());
    alps::hdf5::archive h5(hdf5.string(), "a");
    t.start();
    fipp_start();
    for (std::size_t i = 0; i < N; ++i) {
        h5["/a/" + iv[i]] << obs;
    }
    fipp_stop();
    t.stop();
    std::cout << "HDF5 save:a        " << t.format(6) << std::flush;
    std::cout << "HDF5 size: " << boost::filesystem::file_size(hdf5) << std::endl;
    boost::filesystem::remove(hdf5);
    }
    return 0;
}
