#pragma once

#ifdef ENABLE_VISUALIZATOR

#if (defined _WIN32 || defined _WIN64)
# include <winsock2.h>
#else
#include <sys/socket.h>
#endif

#include <string>
#include <cstdint>
#include <vector>
#include "C_Singleton.h"

namespace AICup
{
  class Visualizator: public Singleton<Visualizator> {
  private:
    static std::string DEFAULT_HOST;
    static std::string DEFAULT_PORT;
    static const int BUF_SIZE = 1024;
    char* buf;
    std::vector<char> bufMemory;

  public:
    enum Style {
      PRE,
      POST,
      ABS,
      UNKNOWN
    };

    bool isReverse;
  public:
    Visualizator();

    Style getStyle() const {
      return style;
    }


    void beginPre();
    void endPre();

    void beginPost();
    void endPost();

    void beginAbs();
    void endAbs();

    void circle(double x, double y, double r, int32_t color = 0x7F7F7F) const;
    void fillCircle(double x, double y, double r, int32_t color = 0x7F7F7F) const;
    void rect(double x1, double y1, double x2, double y2, int32_t color = 0x7F7F7F) const;
    void fillRect(double x1, double y1, double x2, double y2, int32_t color = 0x7F7F7F) const;
    void line(double x1, double y1, double x2, double y2, int32_t color = 0x7F7F7F) const;
    void text(double x, double y, const char* text, int32_t color = 0x7F7F7F) const;
    void text(double x, double y, double value, int32_t color = 0x7F7F7F) const;
    void text(double x, double y, int value, int32_t color = 0x7F7F7F) const;

  private:
    void init();

    double rev(double v) const;

    void sendCommand(const char* str) const;
    void writeWithColor(char* buf, int32_t color) const;

    Style style;
    SOCKET openSocket;
  };

};

#endif