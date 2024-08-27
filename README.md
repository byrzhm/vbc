# vbc

Very basic calculator

```
<expr> ::= <decimal-number>
           | <hexadecimal-number>    # Start with "0x"
           | "(" <expr> ")"
           | <expr> "+" <expr>
           | <expr> "-" <expr>
           | <expr> "*" <expr>
           | <expr> "/" <expr>
           | <expr> "==" <expr>
           | <expr> "!=" <expr>
           | <expr> "&&" <expr>
           | "-" <expr>
```

## References

* [Handling Unary Operations in the Shunting Yard Algorithm](https://inspirnathan.com/posts/155-handling-unary-operations-with-shunting-yard-algorithm)

