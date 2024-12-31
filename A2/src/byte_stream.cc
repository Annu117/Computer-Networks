#include "byte_stream.hh"

#include <algorithm>

using namespace std;

ByteStream::ByteStream(const size_t capa) : capa1(capa) {}

size_t ByteStream::write(const string &data)
{
  if (data.empty() || input_ended())
    return 0;
  size_t len = data.length();
  size_t i = 0;
  while (i < len && buffer.size() < capa1)
  {
    buffer.push_back(data[i]);
    ++i;
  }

  t_write += i;

  if (i < len)
  {
    // This means not all characters from 'data' were written to the buffer
    return i; // Return the number of characters written
  }
  else
  {
    return i;
  }
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const
{
  size_t length = std::min(len, buffer.size());
  return string(buffer.begin(), buffer.begin() + length);
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len)
{
  if (len == 0)
    return;
  if (len > buffer.size())
  {
    set_error();
    return;
  }
  size_t length = std::min(len, buffer.size());

  t_read += length;
  while (length > 0)
  {
    buffer.pop_front();
    --length;
  }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len)
{
  if (len > buffer.size())
  {
    set_error();
    return "";
  }
  string ret = peek_output(len);
  pop_output(ret.size());
  return ret;
}

void ByteStream::end_input() { input_end = true; }

bool ByteStream::input_ended() const { return input_end; }

size_t ByteStream::buffer_size() const { return buffer.size(); }

bool ByteStream::buffer_empty() const { return buffer.empty(); }

bool ByteStream::eof() const { return buffer_empty() && input_ended(); }

size_t ByteStream::bytes_written() const { return t_write; }

size_t ByteStream::bytes_read() const { return t_read; }

size_t ByteStream::remaining_capacity() const { return capa1 - buffer.size(); }
