/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 14:36:31 by bamrouch          #+#    #+#             */
/*   Updated: 2024/01/14 20:16:36 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

File::File(ssize_t &buf_size): buffer_size(buf_size), read_size(0), file_size(0), file_done(false) 
{};

void File::setFilePath(const string &p)
{
    this->path = p;   
};

void File::setFileDone(const bool &done)
{
    this->file_done = done;
};

void File::setReadSize(const ssize_t &size)
{
    this->read_size = size;
};

ssize_t &File::getFileSize()
{
    return file_size;
};

bool File::operator*()
{
    return file_done;
};

void File::operator>>(char (&buf)[HEADERS_MAX_SIZE + 1])
{
    file.open(path.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open())
        throw Response::ResponseException(E_FAILED_RESPONSE_BODY_READ, NULL);
    file.seekg(read_size);
    ssize_t size_to_read = std::min(HEADERS_MAX_SIZE - buffer_size, file_size - read_size);
    read_size += size_to_read;
    file.read(buf + buffer_size, size_to_read);
    buffer_size += size_to_read;
    if (read_size == file_size)
        file_done = true;
    file.close();
}

File::~File()
{
    if (file.is_open())
        file.close();
};