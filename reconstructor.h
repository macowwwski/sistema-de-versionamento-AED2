#pragma once //protege os headers

#include <vector>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include "storage.h"

class Reconstructor {
public:
    Reconstructor(Storage& storage) : storage(storage) {} // construtor

    void reconstruirArquivo(std::string nomeOriginal, std::string nomeSaida){
        // 4. Lógica de Reconstrução

        std::ofstream arquivoSaida(nomeSaida);
        if (!arquivoSaida) {
                throw std::runtime_error("Não foi possível criar o arquivo: " + nomeSaida);
        }

        std::vector<uint32_t> listaIds = storage.obterReceita(nomeOriginal);

        for (uint32_t id : listaIds){
            std::string textoLinha = storage.obterTextoDoBloco(id);
            arquivoSaida << textoLinha << "\n"; // escreve no arquivo ao invés do terminal
        }

        arquivoSaida.close();
    }


private:
    Storage& storage; // se não usasse referência, cada instância de storage seria uma separada

};