#include "relocation.h"

#include <sys/mman.h>

void handleRela(std::vector<ObjectFile> &allObject, ObjectFile &mergedObject, bool isPIE)
{
    /* When there is more than 1 objects, 
     * you need to adjust the offset of each RelocEntry
     */
    /* Your code here */
    int offset = 0;
    for(auto &obj : allObject) {
        for(auto &re : obj.relocTable) {
            re.offset += offset;
        }
        offset += obj.sections[".text"].size;
    }
    
    

    /* in PIE executables, user code starts at 0xe9 by .text section */
    /* in non-PIE executables, user code starts at 0xe6 by .text section */
    uint64_t userCodeStart = isPIE ? 0xe9 : 0xe6;
    uint64_t textOff = mergedObject.sections[".text"].off + userCodeStart;
    uint64_t textAddr = mergedObject.sections[".text"].addr + userCodeStart;

    /* Your code here */
    for(auto &obj : allObject) {
        for(auto &re : obj.relocTable) {

            uint64_t addr = (uint64_t) mergedObject.baseAddr + re.offset + textOff;

            if(isPIE) {
                
                int valueToFill_PIE = re.sym->value - (re.offset + textAddr ) + re.addend;
                *reinterpret_cast<int *>(addr) = valueToFill_PIE;

            }else if(!isPIE) {

                if(re.type == 4)
                {
                    int valueToFill_r = re.sym->value - (re.offset + textAddr ) + re.addend;
                    *reinterpret_cast<int *>(addr) = valueToFill_r; 

                }else {

                    int valueToFill = re.sym->value  + re.addend;
                    *reinterpret_cast<uint32_t *>(addr) = valueToFill;   
                }
                
            }
        }
    }
    
}