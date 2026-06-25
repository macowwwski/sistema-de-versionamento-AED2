#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "storage.h"
#include "deduplicator.h"
#include "reconstructor.h"
#include "serializer.h"

class CommandLineInterface {
public:
    void processarArgumentos(int argc, char* argv[]) {
        if (argc < 3) {
            std::cout << "Uso:\n";
            std::cout << "  dedup ingerir <arquivo1.txt> ...\n";
            std::cout << "  dedup reconstruir <nomeOriginal> <saida.txt>\n";
            return;
        }

        std::string modo = argv[1];

        if (modo == "ingerir") {
            modoIngestao(argc, argv);
        } else if (modo == "reconstruir") {
            modoReconstrucao(argc, argv);
        } else {
            throw std::runtime_error("Modo inválido: " + modo);
        }
    }

private:
    Storage storage;
    Serializer serializer{storage};
    Deduplicator deduplicator{storage};
    Reconstructor reconstructor{storage};

    void modoIngestao(int argc, char* argv[]) {
        // tenta carregar estado anterior do disco
        try {
            serializer.carregarDoDisco("storage.dat");
        } catch (...) {
            // primeira execução, storage.dat ainda n existe
        } 

        // monta a lista de arquivos a partir dos argumentos
        std::vector<std::string> arquivos;
        for (int i = 2; i < argc; i++) {
            arquivos.push_back(argv[i]);
        }

        deduplicator.ingerirArquivos(arquivos);
        serializer.salvarParaDisco("storage.dat");
    }

    void modoReconstrucao(int argc, char* argv[]) {
        if (argc < 4) {
            throw std::runtime_error("Uso: dedup reconstruir <nomeOriginal> <saida.txt>");
        }

        serializer.carregarDoDisco("storage.dat");

        std::string nomeOriginal = argv[2];
        std::string nomeSaida    = argv[3];

        reconstructor.reconstruirArquivo(nomeOriginal, nomeSaida);
    }
};