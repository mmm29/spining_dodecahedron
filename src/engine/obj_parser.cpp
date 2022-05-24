#include "obj_parser.h"

std::shared_ptr<Mesh> ObjParser::Parse(const std::string &text) {
    return Parse(text.c_str(), text.size());
}

std::shared_ptr<Mesh> ObjParser::Parse(const char *text, size_t length) {
    ObjParser parser;
    parser.SetText(text, length);
    if (!parser.Parse())
        return nullptr;

    return parser.GetMesh();
}

void ObjParser::SetText(const char *text, size_t length) {
    text_ = text;
    end_ = text + length;

    cursor_ = text_;
}

bool ObjParser::Parse() {
    ResetCursor();

    const uint32_t vertex_count = CountVertices();

    ResetCursor();

    std::vector<Mesh::Vertex> vertices;
    vertices.resize(vertex_count);

    Mesh::Vertex *vertex = &vertices[0];

    std::vector<Mesh::Face> faces;

    while (!EndReached()) {
        if (Current() == '#') {
            NextLine();
            continue;
        }

        std::string data_type = ReadWord();

        if (data_type == "v") {
            vertex->position[0] = std::stof(ReadWord());
            vertex->position[1] = std::stof(ReadWord());
            vertex->position[2] = std::stof(ReadWord());
            vertex++;
            NextLine();
            continue;
        }

        if (data_type == "f") {
            Mesh::Face face;
            face.indices.reserve(3);

            for (size_t i = 0; i < 3; i++) {
                int index = std::stoi(ReadWord());

                if (index < 0)
                    index += static_cast<int>(vertex_count);
                else
                    index--;

                if (index < 0 || index >= vertex_count)
                    return false;

                face.indices.push_back(index);
            }

            faces.emplace_back(std::move(face));
            NextLine();
            continue;
        }

        NextLine();
    }

    mesh_ = std::make_shared<Mesh>();
    mesh_->SetVertices(std::move(vertices));
    mesh_->SetFaces(std::move(faces));

    return true;
}

uint32_t ObjParser::CountVertices() {
    uint32_t vertex = 0;

    while (!EndReached()) {
        SkipSpaces();

        if (Current() == 'v' && Peek(1) == ' ')
            vertex++;

        NextLine();
    }

    return vertex;
}

std::string ObjParser::ReadWord() {
    std::string word;

    SkipSpaces();

    while (!EndReached()) {
        if (Current() == '\n' || Current() == ' ')
            return word;

        word += Current();
        Next();
    }

    return word;
}

void ObjParser::NextLine() {
    while (!EndReached()) {
        if (Current() == '\n') {
            Next();
            return;
        }

        Next();
    }
}

void ObjParser::Next() {
    if (cursor_ == end_)
        return;

    cursor_++;
}

bool ObjParser::EndReached() const {
    return cursor_ == end_;
}

char ObjParser::Peek(int offset) const {
    const char *p = cursor_ + offset;
    if (p < text_ || p >= end_)
        return '\0';

    return *p;
}

char ObjParser::Current() const {
    return Peek(0);
}

void ObjParser::SkipSpaces() {
    while (!EndReached() && Current() == ' ')
        Next();
}

void ObjParser::ResetCursor() {
    cursor_ = text_;
}

std::shared_ptr<Mesh> ObjParser::GetMesh() const {
    return mesh_;
}