/* Copyright 2003-2017 GBDI-ICMC-USP <caetano@icmc.usp.br>
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*   http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#ifndef __STMULTIPLEPAGEMANAGER_H
#define __STMULTIPLEPAGEMANAGER_H

#include <arboretum/stPageManager.h>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::to_string;
using std::exception;

template <class PageManager>
class stMultiplePageManager : public stPageManager {
public:
    stMultiplePageManager(const string &fileName,
                          u_int32_t pagesPerPM,
                          u_int32_t pageSize);

    stMultiplePageManager(const string &fileName,
                          u_int32_t pagesPerPM);

    virtual ~stMultiplePageManager();

    virtual bool IsEmpty();

    virtual stPage* GetHeaderPage();

    virtual stPage* GetPage(u_int32_t pageID);

    virtual void ReleasePage(stPage *page);

    virtual stPage* GetNewPage();

    virtual void WritePage(stPage *page);

    virtual void WriteHeaderPage(stPage *headerPage);

    virtual void DisposePage(stPage *page);

    virtual u_int32_t GetMinimumPageSize();

    virtual u_int32_t GetPageCount();

    virtual void ResetStatistics();

private:
    string fileName;

    u_int32_t pagesPerPM;

    u_int32_t pageCount;

    vector<PageManager*> PMs;

    u_int32_t GetCorrespondentPM(u_int32_t logicalPageID);

    u_int32_t GetActualPageID(u_int32_t logicalPageID);
};

#include "stMultiplePageManager-inl.h"

#endif
