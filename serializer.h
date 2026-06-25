#pragma once //protege os headers

#include <vector>
#include <iostream>
#include <fstream>      
#include <string>     
#include <unordered_map>
#include "storage.h"

class Serializer {
public:
    Serializer(Storage& storage) : storage(storage) {}

    void salvarParaDisco(std::string caminhoArquivoDat){
        std::ofstream arquivo(caminhoArquivoDat, std::ios::binary);
        if (!arquivo) {
            throw std::runtime_error("Não foi possível criar: " + caminhoArquivoDat);
        }

        salvarDicionario(arquivo);
        salvarRepositorio(arquivo);
        salvarReceitas(arquivo);

        arquivo.close();
    }

    void carregarDoDisco(std::string caminhoArquivoDat){
        std::ifstream arquivo(caminhoArquivoDat, std::ios::binary);
        if (!arquivo) {
            throw std::runtime_error("Não foi possível abrir: " + caminhoArquivoDat);
        }

        carregarDicionario(arquivo);
        carregarRepositorio(arquivo);
        carregarReceitas(arquivo);

        arquivo.close();
    }


private:
    Storage& storage;

    // FUNÇÕES AUXILIARES SALVAR
    void salvarDicionario(std::ofstream& arquivo) { 
        auto& dicionario = storage.getDicionario();
        uint32_t quantidade = dicionario.size();
        arquivo.write(reinterpret_cast<const char*>(&quantidade), sizeof(quantidade));

        for (const auto& par : dicionario) {
            arquivo.write(reinterpret_cast<const char*>(&par.first), sizeof(par.first));
            arquivo.write(reinterpret_cast<const char*>(&par.second), sizeof(par.second));
        }
    }

    void salvarRepositorio(std::ofstream& arquivo) { 
        auto& repositorio = storage.getRepositorio();
        uint32_t quantidade = repositorio.size();
        arquivo.write(reinterpret_cast<const char*>(&quantidade), sizeof(quantidade));

        for (const std::string& texto : repositorio) {
            // salva o tamanho da string antes do conteúdo
            uint32_t tamanho = texto.size();
            arquivo.write(reinterpret_cast<const char*>(&tamanho), sizeof(tamanho));
            arquivo.write(texto.c_str(), tamanho); // escreve os bytes da string
        }
    }

    void salvarReceitas(std::ofstream& arquivo) { 
        auto& receitas = storage.getReceitas();
        uint32_t quantidade = receitas.size();
        arquivo.write(reinterpret_cast<const char*>(&quantidade), sizeof(quantidade));

        for (const auto& par : receitas) {
            // salva o nome do arquivo
            uint32_t tamanhoNome = par.first.size();
            arquivo.write(reinterpret_cast<const char*>(&tamanhoNome), sizeof(tamanhoNome));
            arquivo.write(par.first.c_str(), tamanhoNome);

            // salva a lista de ids
            uint32_t qtdIds = par.second.size();
            arquivo.write(reinterpret_cast<const char*>(&qtdIds), sizeof(qtdIds));
            for (uint32_t id : par.second) {
                arquivo.write(reinterpret_cast<const char*>(&id), sizeof(id));
            }
        }
    }


    // FUNÇÕES AUXILIARES CARREGAR
    void carregarDicionario(std::ifstream& arquivo) { 
        auto& dicionario = storage.getDicionario();
        uint32_t quantidade;
        arquivo.read(reinterpret_cast<char*>(&quantidade), sizeof(quantidade));

        for (uint32_t i = 0; i < quantidade; i++) {
            size_t chave;
            uint32_t valor;
            arquivo.read(reinterpret_cast<char*>(&chave), sizeof(chave));
            arquivo.read(reinterpret_cast<char*>(&valor), sizeof(valor));
            dicionario[chave] = valor;
        }
    }

    void carregarRepositorio(std::ifstream& arquivo) { 
        auto& repositorio = storage.getRepositorio();
        uint32_t quantidade;
        arquivo.read(reinterpret_cast<char*>(&quantidade), sizeof(quantidade));

        for (uint32_t i = 0; i < quantidade; i++) {
            // lê o tamanho da string
            uint32_t tamanho;
            arquivo.read(reinterpret_cast<char*>(&tamanho), sizeof(tamanho));

            // lê exatamente 'tamanho' bytes e reconstrói a string
            std::string texto(tamanho, '\0');
            arquivo.read(&texto[0], tamanho);
            repositorio.push_back(texto);
        }
    }

    void carregarReceitas(std::ifstream& arquivo) {
        auto& receitas = storage.getReceitas();
        uint32_t quantidade;
        arquivo.read(reinterpret_cast<char*>(&quantidade), sizeof(quantidade));

        for (uint32_t i = 0; i < quantidade; i++) {
            // lê o nome do arquivo
            uint32_t tamanhoNome;
            arquivo.read(reinterpret_cast<char*>(&tamanhoNome), sizeof(tamanhoNome));
            std::string nomeArquivo(tamanhoNome, '\0');
            arquivo.read(&nomeArquivo[0], tamanhoNome);

            // lê a lista de ids
            uint32_t qtdIds;
            arquivo.read(reinterpret_cast<char*>(&qtdIds), sizeof(qtdIds));
            std::vector<uint32_t> listaIds(qtdIds);
            for (uint32_t j = 0; j < qtdIds; j++) {
                arquivo.read(reinterpret_cast<char*>(&listaIds[j]), sizeof(uint32_t));
            }

            receitas[nomeArquivo] = listaIds;
        }
    }
};