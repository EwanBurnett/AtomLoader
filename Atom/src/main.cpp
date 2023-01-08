#include <cstdio>
#include "Model.h"
#include "Exporter.h"
#include <thread>

int main(int argc, const char* argv[]) {
    
    //Parse CMD Arguments if available. Return immediately otherwise.
    if (argc > 1) {
        std::vector<std::thread*> threads;

        printf("\nImporting %d models.", argc - 1);

        //Export each model supplied
        for (int i = 1; i < argc; i++) {

            threads.push_back(new std::thread([argv, i]() {
                std::string fileName = argv[i];

                printf("\nExporting model %s to %s.atom", fileName.c_str(), fileName.c_str());
                Atom::Model model;
                model.ImportFromFile(fileName);

                Atom::Exporter::ExportModelToImage(model, fileName + ".atom");
            }));

        }
        
        //Clean up threads
        for (auto* thread : threads) {
            thread->join();
            delete(thread);
        }
    }
    else {
        printf("Error: No Input File Specified!\nUsage: Atom-Loader.exe filename");
    }
    
    return 0;
  
}
