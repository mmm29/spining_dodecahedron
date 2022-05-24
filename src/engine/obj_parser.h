#pragma once

#include <memory>
#include <string>

#include "mesh.h"

class ObjParser {
public:
    static std::shared_ptr<Mesh> Parse(const std::string &text);

    static std::shared_ptr<Mesh> Parse(const char *text, size_t length);

    void SetText(const char *text, size_t length);

    bool Parse();

    std::shared_ptr<Mesh> GetMesh() const;

private:
    void NextLine();

    void Next();

    bool EndReached() const;

    char Peek(int offset) const;

    char Current() const;

    void SkipSpaces();

    void ResetCursor();

    uint32_t CountVertices();

    std::string ReadWord();

private:
    const char *text_;
    const char *end_;

    const char *cursor_;

private:
    std::shared_ptr<Mesh> mesh_;
};