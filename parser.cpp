#include"parser.h"
#include<cassert>

Parser::Parser(std::deque<TokenPtr> tokens) : Token_data{std::move(tokens)} {}

ValuePtr Parser::parse() {
    if (Token_data.empty()) throw SyntaxError("No More Tokens There");
    auto token = std::move(Token_data.front());
    Token_data.pop_front();
    if (token->getType() == TokenType::NUMERIC_LITERAL) {
        auto value = static_cast<NumericLiteralToken&>(*token).getValue();
        return std::make_shared<NumericValue>(value);
    } else if (token->getType() == TokenType::BOOLEAN_LITERAL) {
        auto value = static_cast<BooleanLiteralToken&>(*token).getValue();
        return std::make_shared<BooleanValue>(value);
    } else if (token->getType() == TokenType::STRING_LITERAL) {
        auto value = static_cast<StringLiteralToken&>(*token).getValue();
        return std::make_shared<StringValue>(value);
    } else if (token->getType() == TokenType::IDENTIFIER) {
        auto value = static_cast<IdentifierToken&>(*token).getName();
        return std::make_shared<SymbolValue>(value);
    } else if (token->getType() == TokenType::LEFT_PAREN) {
        auto value = parseTails();
        return value;
    } else if (token->getType() == TokenType::QUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("quote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    } else if (token->getType() == TokenType::QUASIQUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("quasiquote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    } else if (token->getType() == TokenType::UNQUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("unquote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    }
    else throw SyntaxError("Unimplemented");
}

ValuePtr Parser::parseTails() {
    if (Token_data.empty()) 
        throw SyntaxError("There's No Right_Paren");

    if (Token_data.front()->getType() == TokenType::RIGHT_PAREN) {
        Token_data.pop_front();
        return std::make_shared<NilValue>();
    }
    auto car = this->parse();
    if (Token_data.empty()) throw SyntaxError("There's No Right_Paren");
    if (Token_data.front()->getType() == TokenType::DOT) {
        Token_data.pop_front();
        auto cdr = this->parse();
        auto next_token = std::move(Token_data.front());

        if(next_token->getType()!=TokenType::RIGHT_PAREN) 
            throw SyntaxError("More Than One Elements After a Dot");

        Token_data.pop_front();
        return std::make_shared<PairValue>(car, cdr);
    } else {
        auto cdr = this->parseTails();
        return std::make_shared<PairValue>(car, cdr);
    }
}
