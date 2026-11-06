#ifndef MAIL_SPOOLER_H
#define MAIL_SPOOLER_H

#include <string>
#include <vector>
#include <filesystem>

class MailSpooler {
private:
    std::string spoolDir_;
    std::filesystem::path getUserDir(const std::string& username) const;
    std::vector<std::filesystem::path> getSortedUserFiles(const std::string& username) const;

public:
    explicit MailSpooler(const std::string& spoolDir);
    bool saveMessage(const std::string& sender, const std::string& receiver, const std::string& subject, const std::string& message);
    std::vector<std::string> listSubjects(const std::string& username);
    bool readMessage(const std::string& username, int messageNum, std::string& outSender, std::string& outReceiver, std::string& outSubject, std::string& outContent);
    bool deleteMessage(const std::string& username, int messageNum);
};

#endif // MAIL_SPOOLER_H