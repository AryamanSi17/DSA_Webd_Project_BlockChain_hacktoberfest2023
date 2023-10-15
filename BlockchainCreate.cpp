#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <openssl/sha.h>

class Block {
public:
    int index;
    std::string previousHash;
    std::string data;
    std::string timestamp;
    std::string hash;
    int nonce;

    Block(int index, const std::string& previousHash, const std::string& data)
        : index(index), previousHash(previousHash), data(data) {
        timestamp = getCurrentTimestamp();
        nonce = 0;
        hash = calculateHash();
    }

    // Proof of Work (Mining)
    void mineBlock(int difficulty) {
        std::string target(difficulty, '0');
        while (hash.substr(0, difficulty) != target) {
            nonce++;
            hash = calculateHash();
        }
        std::cout << "Block mined: " << hash << std::endl;
    }

private:
    std::string calculateHash() {
        std::string dataToHash = std::to_string(index) + previousHash + data + timestamp + std::to_string(nonce);
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((const unsigned char*)dataToHash.c_str(), dataToHash.length(), hash);
        std::string hashStr;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            char hex[3];
            sprintf(hex, "%02x", hash[i]);
            hashStr += hex;
        }
        return hashStr;
    }

    std::string getCurrentTimestamp() {
        std::time_t currentTime = std::time(nullptr);
        return std::asctime(std::localtime(&currentTime));
    }
};

class Blockchain {
public:
    Blockchain() {
        chain.emplace_back(createGenesisBlock());
        difficulty = 4; // Adjust the difficulty as needed.
    }

    void addBlock(const std::string& data) {
        Block newBlock(chain.size(), getLatestBlock().hash, data);
        newBlock.mineBlock(difficulty);
        chain.push_back(newBlock);
    }

private:
    Block createGenesisBlock() {
        return Block(0, "0", "Genesis Block");
    }

    Block getLatestBlock() const {
        return chain.back();
    }

    std::vector<Block> chain;
    int difficulty;
};

int main() {
    Blockchain myBlockchain;

    std::cout << "Mining block 1..." << std::endl;
    myBlockchain.addBlock("Transaction data 1");

    std::cout << "Mining block 2..." << std::endl;
    myBlockchain.addBlock("Transaction data 2");

    return 0;
}
