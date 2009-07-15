/***************************************************************************
 *   MP3 Diags - diagnosis, repairs and tag editing for MP3 files          *
 *                                                                         *
 *   Copyright (C) 2009 by Marian Ciobanu                                  *
 *   ciobi@inbox.com                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 2 as     *
 *   published by the Free Software Foundation.                            *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

//#include  <QSettings> //ttt0
//#include  <iostream>

#include  "fstream_unicode.h"

#include  <sys/stat.h>


// might want to look at basic_file_stdio.cc or ext/stdio_filebuf.h

// http://www2.roguewave.com/support/docs/leif/sourcepro/html/stdlibref/basic-ifstream.html
// STLPort
// MSVC
// Dinkumware - not

// http://www.aoc.nrao.edu/~tjuerges/ALMA/STL/html/class____gnu__cxx_1_1stdio__filebuf.html




//#include  "Helpers.h" //ttt remove

//ttt0 perhaps add flush()


int getAcc(std::ios_base::openmode __mode)
{
    int nAcc;
    if (std::ios_base::in & __mode)
    {
        if (std::ios_base::out & __mode)
        {
            nAcc = O_RDWR | O_CREAT;
        }
        else
        {
            nAcc = O_RDONLY;
        }
    }
    else if (std::ios_base::out & __mode)
    {
        nAcc = O_RDWR | O_CREAT;
    }
    else
    {
        throw 1; // ttt0
    }

    #ifdef O_LARGEFILE
    nAcc = nAcc | O_LARGEFILE;
    #endif

    if (std::ios_base::trunc & __mode)
    {
        nAcc = nAcc | O_TRUNC;
    }

    #ifdef O_BINARY
    if (std::ios_base::binary & __mode)
    {
        nAcc = nAcc | O_BINARY;
    }
    #endif

    return nAcc;
}


#ifndef WIN32

    template<>
    int unicodeOpenHlp(const char* szUtf8Name, std::ios_base::openmode __mode)
    {
        int nAcc (getAcc(__mode));
        int nFd (open(szUtf8Name, nAcc, S_IREAD | S_IWRITE));
        //qDebug("fd %d %s acc=%d", nFd, szUtf8Name, nAcc);
        return nFd;
    }

    template<>
    int unicodeOpenHlp(const wchar_t* /*wszUtf16Name*/, std::ios_base::openmode /*__mode*/)
    {
        throw 1; //ttt2 add if needed
    }


#else


    #include  <windows.h>

    #include  <vector>
    #include  <string>

    using namespace std;
    using namespace __gnu_cxx;


    static wstring wstrFromUtf8(const string& s) //ttt1 perhaps public; maybe not: since both constructors are available, the user can call without needing any conversion
    {
        vector<wchar_t> w (s.size() + 1);
        MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &w[0], w.size());
        //inspect(&w[0], w.size()*2);
        return &w[0];
    }


    template<>
    int unicodeOpenHlp(const char* szUtf8Name, std::ios_base::openmode __mode)
    {
        return unicodeOpenHlp(wstrFromUtf8(szUtf8Name).c_str());
    }

    template<>
    int unicodeOpenHlp(const wchar_t* wszUtf16Name, std::ios_base::openmode __mode)
    {
        int nAcc (getAcc(__mode));

        int nFd (_wopen(wszUtf16Name, nAcc, S_IREAD | S_IWRITE));
        //int nFd (open(szUtf8Name, nAcc, S_IREAD | S_IWRITE));
        //qDebug("fd %d %s acc=%d", nFd, szUtf8Name, nAcc);
        return nFd;
    }

#endif // #ifndef WIN32 / #else


template<>
int unicodeOpenHlp(char* szUtf8Name, std::ios_base::openmode __mode)
{
    return unicodeOpenHlp<const char*>(szUtf8Name, __mode);
}

template<>
int unicodeOpenHlp(wchar_t* wszUtf8Name, std::ios_base::openmode __mode)
{
    return unicodeOpenHlp<const wchar_t*>(wszUtf8Name, __mode);
}


template<>
int unicodeOpenHlp(int fd, std::ios_base::openmode /*__mode*/)
{
    return fd;
}





void wefwef()
{
    ifstream_unicode in1 ("abc");
    in1.open("abc");
}

//ttt0 review O_SHORT_LIVED