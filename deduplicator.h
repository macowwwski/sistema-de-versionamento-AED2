#pragma once //protege os headers

#include <vector>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include <functional>
#include "storage.h"


class Deduplicator {
public:
    void ingerirArquivos(std::vector<std::string> listaNomesArquivos){
        
        // 2. Chunking
        for (std::string nomeArquivo : listaNomesArquivos){
            
            std::ifstream arquivo(nomeArquivo);
            if (!arquivo) {
                throw std::runtime_error("Não foi possível abrir o arquivo: " + nomeArquivo);
            }

            std::vector<uint32_t> listaIds;
            std::string linha;
            std::hash<std::string> hashing; // define o cálculo de hash para string

            while (std::getline(arquivo, linha)) {
                size_t hash = hashing(linha);   // calcula o hash para a linha

        // 3. Lógica de Ingestão
                uint32_t id = storage.adicionarBlocoSeInedito(hash, linha); // recebe o id associado ao hash
                listaIds.push_back(id); // acumula id de cada linha
            }

            storage.salvarReceita(nomeArquivo, listaIds);   // após ler um arquivo inteiro, salva seu nome e lista de ids
            arquivo.close();
        }
};

private: 
    Storage& storage; // se não usasse referência, cada instância de storage seria uma separada
    
};