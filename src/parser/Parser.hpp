#pragma once

#include "parser/Lexer.hpp"

namespace Parser {
    using Key = std::variant<double, std::string>;
    using RawValue = std::variant<double, bool, std::string, std::vector<double>, std::vector<bool>, std::vector<std::string>>;

    enum class ValueType {
        NUMBER,
        BOOL,
        STRING,
        NUMBER_LIST,
        BOOL_LIST,
        STRING_LIST,
        NODE
    };

    class Node {
        public:
            Node();
            Node(const Node& node);
            Node(const RawValue& value);
            Node(const std::map<Key, Node>& values);

            ValueType GetType() const;
            bool Is(ValueType type) const;
            bool IsList() const;

            // Functions to use with LeafHolder.
            void Push(const RawValue& value);

            // Functions to use with NodeHolder.
            Node& Get(const Key& key);
            const Node& Get(const Key& key) const;
            std::map<Key, Node>& GetEntries();
            // std::vector<Node&> GetValues();
            std::vector<Key> GetKeys() const;
            bool ContainsKey(const Key& key) const;
            void Put(const Key& key, const Node& node);

            // Overload cast for LeafHolder.
            operator int() const;
            operator double() const;
            operator bool() const;
            operator std::string() const;
            operator std::vector<double>&() const;
            operator std::vector<bool>&() const;
            operator std::vector<std::string>&() const;
            operator RawValue&() const;

            Node& operator=(const RawValue& value);
            Node& operator=(const Node& value);

            Node& operator [](const Key& key);
            const Node& operator [](const Key& key) const;
        
        private:
            // Function to access the underlying value holder.
            SharedPtr<NodeHolder> GetNodeHolder();
            const SharedPtr<NodeHolder> GetNodeHolder() const;

            SharedPtr<LeafHolder> GetLeafHolder();
            const SharedPtr<LeafHolder> GetLeafHolder() const;

        private:
            SharedPtr<AbstractValueHolder> m_Value;
    };

    class AbstractValueHolder {
        public:
            virtual ValueType GetType() const = 0;
            virtual SharedPtr<AbstractValueHolder> Copy() const = 0;
    };

    class NodeHolder : public AbstractValueHolder {
        friend Node;

        public:
            NodeHolder();
            NodeHolder(const NodeHolder& n);
            NodeHolder(const std::map<Key, Node>& values);

            virtual ValueType GetType() const;
            virtual SharedPtr<AbstractValueHolder> Copy() const;

        private:
            std::map<Key, Node> m_Values;
    };

    class LeafHolder : public AbstractValueHolder {
        friend Node;

        public:
            LeafHolder();
            LeafHolder(const LeafHolder& l);
            LeafHolder(const RawValue& value);

            virtual ValueType GetType() const;
            virtual SharedPtr<AbstractValueHolder> Copy() const;

        private:
            RawValue m_Value;
    };

    Node Parse(const std::string& content);
    Node Parse(std::deque<PToken>& tokens);
    
    namespace Impl {
        Node ParseNode(std::deque<PToken>& tokens);
        Node ParseRaw(PToken token);
        Node ParseRange(std::deque<PToken>& tokens);

        template<typename T>
        Node ParseList(std::deque<PToken>& tokens);

        bool IsList(std::deque<PToken>& tokens);
    }

    void Benchmark();
}