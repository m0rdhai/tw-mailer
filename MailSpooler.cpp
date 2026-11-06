#include "MailSpooler.h"

#include <fstream>
#include <algorithm>

std::filesystem::path MailSpooler::getUserDir(const std::string& username) const {
    return std::filesystem::path(spoolDir_) / username;
}

std::vector<std::filesystem::path> MailSpooler::getSortedUserFiles(const std::string& username) const {
    std::vector<std::filesystem::path> files;
    std::filesystem::path userPath = getUserDir(username);

    if (!std::filesystem::exists(userPath)) {
        return files;
    }

    for (const auto& entry : std::filesystem::directory_iterator(userPath)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path());
        }
    }

    std::sort(files.begin(), files.end(), [](const std::filesystem::path& a, const std::filesystem::path& b) {
        try {
            return std::stoi(a.stem().string()) < std::stoi(b.stem().string());
        } catch (...) {
            return a.filename() < b.filename();
        }
    });

    return files;
}

MailSpooler::MailSpooler(const std::string& spoolDir) : spoolDir_(spoolDir) {
    if (!std::filesystem::exists(spoolDir_)) {
        std::filesystem::create_directories(spoolDir_);
    }
}

bool MailSpooler::saveMessage(const std::string& sender, const std::string& receiver,
                             const std::string& subject, const std::string& message) {
    try {
        std::filesystem::path userPath = getUserDir(receiver);
        if (!std::filesystem::exists(userPath)) {
            std::filesystem::create_directories(userPath);
        }

        int maxId = 0;
        for (const auto& entry : std::filesystem::directory_iterator(userPath)) {
            if (entry.is_regular_file()) {
                try {
                    int id = std::stoi(entry.path().stem().string());
                    if (id > maxId) maxId = id;
                } catch (...) {}
            }
        }

        std::filesystem::path filePath = userPath / (std::to_string(maxId + 1) + ".txt");
        std::ofstream outFile(filePath);
        if (!outFile.is_open()) return false;

        outFile << sender << "\n" << receiver << "\n" << subject << "\n" << message;
        outFile.close();
        return true;
    } catch (...) {
        return false;
    }
}

std::vector<std::string> MailSpooler::listSubjects(const std::string& username) {
    std::vector<std::string> subjects;
    std::vector<std::filesystem::path> files = getSortedUserFiles(username);

    for (const auto& file : files) {
        std::ifstream inFile(file);
        if (inFile.is_open()) {
            std::string sender, receiver, subject;
            std::getline(inFile, sender);
            std::getline(inFile, receiver);
            std::getline(inFile, subject);
            subjects.push_back(subject);
        }
    }
    return subjects;
}

bool MailSpooler::readMessage(const std::string& username, int messageNum, std::string& outSender, std::string& outReceiver, std::string& outSubject, std::string& outContent) {
    std::vector<std::filesystem::path> files = getSortedUserFiles(username);

    if (messageNum <= 0 || messageNum > static_cast<int>(files.size())) {
        return false;
    }

    std::ifstream inFile(files[messageNum - 1]);
    if (!inFile.is_open()) return false;

    std::getline(inFile, outSender);
    std::getline(inFile, outReceiver);
    std::getline(inFile, outSubject);

    std::string line;
    outContent.clear();
    while (std::getline(inFile, line)) {
        outContent += line + "\n";
    }
    inFile.close();
    return true;
}

bool MailSpooler::deleteMessage(const std::string& username, int messageNum) {
    std::vector<std::filesystem::path> files = getSortedUserFiles(username);

    if (messageNum <= 0 || messageNum > static_cast<int>(files.size())) {
        return false;
    }

    return std::filesystem::remove(files[messageNum - 1]);
}