#ifndef SYMTABLE_H
#define SYMTABLE_H


/*****************************************************************************/
/*                     Symbol tables used in type matching                   */
/*****************************************************************************/

enum symtable_entry {
  Unbound,
  SizeEntry,
  SymbolEntry,
  TypeEntry
};

typedef struct {
  enum symtable_entry tag;
  union {
    size_t SizeEntry;
    const char *SymbolEntry;
    const ndt_t *TypeEntry;
  };
} symtable_entry_t;

typedef struct symtable {
    symtable_entry_t entry;
    struct symtable *next[];
} symtable_t;

symtable_t *symtable_new(ndt_context_t *ctx);
void symtable_del(symtable_t *t);
int symtable_add(symtable_t *t, const char *key, const symtable_entry_t entry,
                 ndt_context_t *ctx);
symtable_entry_t symtable_find(const symtable_t *t, const char *key);


#endif /* SYMTABLE_H */
