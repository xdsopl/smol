Work in small, reviewable steps:
- One logical change per commit. Stop after each commit and wait for me.
- Do not rename anything unless I asked for that specific rename.
- Do not add, remove or reword comments unless I ask.
- Do not reformat or touch lines unrelated to the change.
- Do not start parallel or background tasks or subagents.
- If a change would touch more than ~3 files, or you're unsure about scope, ask first.
- Before editing, tell me in one or two sentences what you will change.
- Do not change the bitstream format or the encoder output unless I ask. `./smol e` must produce the same bytes as before.
- Build with `make` (no warnings with `-W -Wall`) and check the round trip with `make test`. For decoder changes, also build with `gcc -g -fsanitize=address,undefined` and feed `./smol d` random bytes.

Code style (match the surrounding code):
- C99. Declare variables where they are first needed, also in the middle of a block and in `for (int i = 0; ...)`, including two at once (`for (int i = 0, sum = 0; ...)`).
- Indent with tabs, one tab per level, tab width 8. Never indent with spaces.
- Function definitions: opening brace on the same line as the signature, `int main(int argc, char **argv) {`. Functions without parameters are written `()`, not `(void)`.
- if/while/for/else: brace on the same line, `} else {` and `} else if (...) {`.
- A single-statement body gets no braces and sits on its own line, indented one level. Nested for/if/else-if chains stack without braces (see print_table() in mtf.h). If one branch of an if/else needs braces, all branches get them, even single statements (see putrle() and getrle() in rle.h).
- Space after if/while/for, no space before the parentheses of a call. The pointer star binds to the name: `unsigned char *p`, `char **argv`. Casts have no space after them: `(int)fwrite(...)`, `*(const int *)a`.
- Binary operators have spaces around them, also inside indexes: `1 << order[ctx]`, `table[i - 1]`, `(x + i) % bwt_length`.
- Prefer prefix `++i` and `--count` as statements; postfix only inside expressions (`count[input[i]]++`, `cnt++`).
- Names are short and lowercase. Functions are snake_case or run together (`getbit`, `putval`, `getrle`, `read_bits`, `mtf_symbol`). Macros are UPPER_CASE (`BLOCK_SIZE`, `ALPHABET_SIZE`).
- Only `int` and `unsigned char` (for byte buffers); no structs, typedefs or enums.
- State lives in `static` variables: file-scope ones prefixed with the module name (`bwt_input`, `bytes_read`), or function-local ones (`static int acc, cnt;` in getbit(), `static int order[2];` in getval()). Large buffers are `static` arrays sized by a macro (`static int pref[BLOCK_SIZE];`), never malloc.
- Every module is a header (`#pragma once`) with its functions defined in it, included by smol.c, the only .c file. System includes `<...>` come before local `"..."` ones.
- Errors: functions return 0 on success and -1 on error; readers return the value (>= 0) or -1. Check every call that can fail with its own `if (...)` and an early `return -1;`, one call per check (see smol.c and vli.h). main() returns 1 on error.
- Validate at the top of a function, one `if` per check, each with an early `return`.
- Messages go to stderr with fprintf, lowercase, no trailing period: `"reached end of stream\n"`.
- There are no comments in the code. Every file starts with a `/* */` header: a title line, a blank line, the copyright line. Leave headers alone.
- Do not wrap or re-wrap lines. Apart from the MTF table in mtf.h, lines are under about 85 columns, except the final fprintf in smol.c.
- Keep each file's ending as it is. The headers end with one blank line after the last line; smol.c does not.

Commit messages (match the existing history):
- A short lowercase subject, no trailing period, under about 60 characters, saying what changed. Example: `limit block sizes to powers of 8, 9, 10 and 11`.
- No body unless the reason is not obvious from the diff.
- No Co-Authored-By, Claude-Session or other trailers, and no links.
- Author commits as `Claude <noreply@anthropic.com>`. Never use my name or email.
