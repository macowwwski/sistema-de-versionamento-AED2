#pragma once //protege os headers

#include <vector>
#include <iostream>
#include <unordered_map>


class Storage {
public:
    uint32_t adicionarBlocoSeInedito(size_t hash, std::string texto){
        if (dicionario_global.count(hash) == 1){    // hash encontrada
            return dicionario_global[hash];
        }
        else {  // hash inédita -> adicionar
            uint32_t novoId = repositorio_global.size(); // pega a próxima posição vazia como id
            dicionario_global[hash] = novoId;   // registra no dicionário
            repositorio_global.push_back(texto);    // registra no repositório
            return novoId;
        }
    };

    std::string obterTextoDoBloco(uint32_t id){
        if (id < repositorio_global.size()){
            return repositorio_global[id]; 
        }
        else {
            throw std::out_of_range("ID inválido: " + std::to_string(id));
        }
    };

    void salvarReceita(std::string nomeArquivo, std::vector<uint32_t> listaIds){  
        /* Só associar listaIds ao arquivo. 
        Quem monta listaIds é a classe Deduplicator*/

        receitas[nomeArquivo] = listaIds;
    };

    std::vector<uint32_t> obterReceita(std::string nomeArquivo){
        if (receitas.count(nomeArquivo) == 0){
            throw std::runtime_error("Receita não encontrada: " + nomeArquivo);
        }
        else {  // receita encontrada
            return receitas[nomeArquivo];
        }
    };

    // Getters para referências das estruturas - usados no Serializer
    std::unordered_map<size_t, uint32_t>& getDicionario() {
    return dicionario_global;
    }

    std::vector<std::string>& getRepositorio() {
        return repositorio_global;
    }

    std::unordered_map<std::string, std::vector<uint32_t>>& getReceitas() {
        return receitas;
    }

private: 
    // Declaração das estruturas de memória principais 
    std::unordered_map<size_t, uint32_t> dicionario_global; // <assinatura hash, id do pedaço>

    std::vector<std::string> repositorio_global; // índice é o id e valor é a string

    std::unordered_map<std::string, std::vector<uint32_t>> receitas; // <nome do arquivo, vetor com ids>
};
