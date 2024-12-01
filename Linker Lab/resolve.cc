#include "resolve.h"

#include <iostream>
#include <unordered_map>

#define FOUND_ALL_DEF 0
#define MULTI_DEF 1
#define NO_DEF 2

std::string errSymName;

int callResolveSymbols(std::vector<ObjectFile> &allObjects);

void resolveSymbols(std::vector<ObjectFile> &allObjects) {
    int ret = callResolveSymbols(allObjects);
    if (ret == MULTI_DEF) {
        std::cerr << "multiple definition for symbol " << errSymName << std::endl;
        abort();
    } else if (ret == NO_DEF) {
        std::cerr << "undefined reference for symbol " << errSymName << std::endl;
        abort();
    }
}

/* bind each undefined reference (reloc entry) to the exact valid symbol table entry
 * Throw correct errors when a reference is not bound to definition,
 * or there is more than one definition.
 */
int callResolveSymbols(std::vector<ObjectFile> &allObjects)
{
    /* Your code here */
    // if found multiple definition, set the errSymName to problematic symbol name and return MULTIDEF;
    // if no definition is found, set the errSymName to problematic symbol name and return NODEF;

    // Symbol Location
    // test4 + test5
    bool over = true;
    for(auto &obj : allObjects) {
        for(auto &re : obj.relocTable) {
            for(auto &re_syl : obj.symbolTable) {
                if(re.sym->name == re_syl.name && re_syl.bind == STB_GLOBAL && re_syl.index == SHN_COMMON)
                {
                    for(auto &oj : allObjects) {
                        for(auto &syl : oj.symbolTable) {
                            if(re.sym->name == syl.name && syl.bind == STB_GLOBAL && (syl.index != SHN_UNDEF && syl.index != SHN_COMMON))
                            {
                                re.sym = &syl;
                                break;
                            } else if(re.sym->name == syl.name && syl.bind == STB_GLOBAL && syl.index == SHN_COMMON)
                            {
                                re.sym = &syl;
                                over = false;
                            }
                        }
                    }
                }
            }   
        }
    }

    // Error check test2 + test3 Intial Code 
    
    for(auto &obj : allObjects) {
        for(auto &re : obj.relocTable) {

            bool flag = true;
            int syl_num = 0;

            for(auto &oj : allObjects) {
                for(auto &syl : oj.symbolTable) {
                    if(re.sym->name == syl.name && syl.bind == STB_GLOBAL && (syl.index != SHN_UNDEF && syl.index != SHN_COMMON))
                    {
                        syl_num++;
                        flag = false;
                    }
                }
            }

            if(flag && over)
            {
                errSymName = re.sym->name;
                return NO_DEF;
            }
            
            if(syl_num > 1)
            {
                errSymName = re.sym->name;
                return MULTI_DEF;
            }
        }
    }
    
    return FOUND_ALL_DEF;
}
