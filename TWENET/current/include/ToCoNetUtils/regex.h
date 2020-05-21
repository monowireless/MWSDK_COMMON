#ifndef REGEX_H
#define REGEX_H

#ifdef __cplusplus
extern "C" {
#endif // C++

extern char *REGEX_comp(char *);
extern bool_t REGEX_exec(char *);
extern void REGEX_modw(char *);
extern bool_t REGEX_subs(char *, char *);

#ifdef __cplusplus
}
#endif // C++

#endif /* REGEX_H */
