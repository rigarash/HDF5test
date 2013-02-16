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

static std::size_t const N = 20000;
static std::size_t const SIZE = 1<<24;

void vector_test() {
    boost::timer::cpu_timer t;
    t.stop();

    // Preparing large random vector
    boost::random::mt19937 rng;
    boost::random::uniform_01<> dst;
    std::vector<double> vec(SIZE);
    BOOST_FOREACH(double& val, vec) {
        val = dst(rng);
    }

    {
    boost::filesystem::path xdr(boost::filesystem::unique_path());
    alps::OXDRFileDump odp(xdr);
    t.start();
    odp << vec;
    t.stop();
    std::cout << "XDR vector save:    " << t.format(6) << std::flush;
    std::cout << "XDR vector size:    " << boost::filesystem::file_size(xdr) << std::endl;
    boost::filesystem::remove(xdr);
    }

    {
    boost::filesystem::path hdf5(boost::filesystem::unique_path());
    alps::hdf5::archive h5(hdf5.string(), "a");
    t.start();
    h5["a"] << vec;
    t.stop();
    std::cout << "HDF5 vector save:a  " << t.format(6) << std::flush;
    std::cout << "HDF5 vector size:   " << boost::filesystem::file_size(hdf5) << std::endl;
    boost::filesystem::remove(hdf5);
    }
}

int main(int argc, char** argv) {
    boost::timer::cpu_timer t;

    alps::ObservableSet obs;
    obs << alps::RealObservable("a");
    obs.reset(true);

    // Assuming that the source directory is "../HDF5test/"
    boost::filesystem::path dump("../HDF5test/test.xdr");
    alps::IXDRFileDump idp(dump);
    t.start();
        obs.load(idp);
    t.stop();
    std::cout << obs;
    std::cout << "XDR load:          " << t.format(6) << std::flush;

    // vector test
    vector_test();

    // prepare string of i
    std::vector<std::string> iv(N);
    for (std::size_t i = 0; i < N; ++i) {
        iv[i] = boost::lexical_cast<std::string>(i);
    }

    boost::filesystem::path xdr(boost::filesystem::unique_path());
    alps::OXDRFileDump odp(xdr);
    t.start();
    for (std::size_t i = 0; i < N; ++i) {
        obs.save(odp);
    }
    t.stop();
    std::cout << "XDR save:          " << t.format(6) << std::flush;
    std::cout << "XDR size:  " << boost::filesystem::file_size(xdr) << std::endl;
    boost::filesystem::remove(xdr);

    {
    boost::filesystem::path hdf5(boost::filesystem::unique_path());
    alps::hdf5::archive h5(hdf5.string(), "a");
    t.start();
    for (std::size_t i = 0; i < N; ++i) {
        h5["/a/" + iv[i]] << obs;
    }
    t.stop();
    std::cout << "HDF5 save:a        " << t.format(6) << std::flush;
    std::cout << "HDF5 size: " << boost::filesystem::file_size(hdf5) << std::endl;
    boost::filesystem::remove(hdf5);
    }

    {
    boost::filesystem::path hdf5(boost::filesystem::unique_path());
    alps::hdf5::archive h5(hdf5.string(), "a");
    t.start();
    for (std::size_t i = 0; i < N; ++i) {
        h5["/a/a/" + iv[i]] << obs;
    }
    t.stop();
    std::cout << "HDF5 save:a/a      " << t.format(6) << std::flush;
    std::cout << "HDF5 size: " << boost::filesystem::file_size(hdf5) << std::endl;
    boost::filesystem::remove(hdf5);
    }

    {
    boost::filesystem::path hdf5(boost::filesystem::unique_path());
    alps::hdf5::archive h5(hdf5.string(), "a");
    t.start();
    for (std::size_t i = 0; i < N; ++i) {
        h5["/a/a/a/" + iv[i]] << obs;
    }
    t.stop();
    std::cout << "HDF5 save:a/a/a    " << t.format(6) << std::flush;
    std::cout << "HDF5 size: " << boost::filesystem::file_size(hdf5) << std::endl;
    boost::filesystem::remove(hdf5);
    }

    {
    boost::filesystem::path hdf5(boost::filesystem::unique_path());
    alps::hdf5::archive h5(hdf5.string(), "a");
    t.start();
    for (std::size_t i = 0; i < N; ++i) {
        h5["/a/a/a/a/" + iv[i]] << obs;
    }
    t.stop();
    std::cout << "HDF5 save:a/a/a/a  " << t.format(6) << std::flush;
    std::cout << "HDF5 size: " << boost::filesystem::file_size(hdf5) << std::endl;
    boost::filesystem::remove(hdf5);
    }

    {
    boost::filesystem::path hdf5(boost::filesystem::unique_path());
    alps::hdf5::archive h5(hdf5.string(), "a");
    t.start();
    for (std::size_t i = 0; i < N; ++i) {
        h5["/a/a/a/a/a/" + iv[i]] << obs;
    }
    t.stop();
    std::cout << "HDF5 save:a/a/a/a/a" << t.format(6) << std::flush;
    std::cout << "HDF5 size: " << boost::filesystem::file_size(hdf5) << std::endl;
    boost::filesystem::remove(hdf5);
    }
    return 0;
}
