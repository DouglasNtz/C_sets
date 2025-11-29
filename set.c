#include "set.h"

void set_init(Set *set, int (*match)(const void *key1, const void *key2), void (*destroy)(void *data)) {

    list_init(set, destroy);
    set->match = match;
    
    return;
}

int set_insert(Set *set, const void *data) {

    if (set_is_member(set, data) == 1)
        return 1;

    return list_ins_next(set, set->tail, data);

}

int set_remove(Set *set, void **data) {

    ListElmt *member, *prev = NULL;

    for (member = set->head; member != NULL; member = member->next) {

        if ((set->match)(member->data, *data) == 1) 
            break;

        prev = member;

    }
    
    if (member == NULL)
        return -1;

    return list_rem_next(set, prev, data);
}

int set_union(Set *setu, const Set *set1, const Set *set2) {

    ListElmt *member;

    set_init(setu, set1->match, set1->destroy);

    for (member = set1->head; member != NULL; member = member->next) {

        if (list_ins_next(setu, setu->tail, member->data) == -1) {  /* Mais econômico que set_insert */
            set_destroy(setu);
            return -1;
        }
    }

    for (member = set2->head; member != NULL; member = member->next) {

        if (set_insert(setu, member->data) == -1) {  /* Aqui usamos set_insert, que testa antes de inserir na cauda */
            set_destroy(setu);
            return -1;
        }
    }

    return 0;

}

int set_intersection(Set *seti, const Set *set1, const Set *set2) {

    ListElmt *member;

    set_init(seti, set1->match, set1->destroy);

    for (member = set1->head; member != NULL; member = member->next) {

        if (set_is_member(set2, member->data)) /* insere apenas quem também está em set2 */

            if (list_ins_next(seti, seti->tail, member->data) == -1) {
                set_destroy(seti);
                return -1;
            }
    }

    return 0;

}

int set_diference(Set *setd, const Set *set1, const Set *set2) {

    ListElmt *member;

    set_init(setd, set1->match, set1->destroy);

    for (member = set1->head; member != NULL; member = member->next) {

        if (!set_is_member(set2, member->data)) /* Insere apenas quem não está em set2 */

            if (list_ins_next(setd, setd->tail, member->data) == -1) {
                set_destroy(setd);
                return -1;
            }
    }

    return 0;

}

int set_is_subset(const Set *set1, const Set *set2) {

    ListElmt *member;

    if (set1->size > set2->size)  /* Teste rápido, ajuda não testar elemento por elemento */
        return 0;

    for (member = set1->head; member != NULL; member = member->next) {

        if (!set_is_member(set2, member->data))
            return 0;
    }

    return 1;

}

int set_is_equal(const Set *set1, const Set *set2) {

    if (set1->size != set2->size)
        return 0;

    return set_is_subset(set1, set2);  /* Se são do mesmo tamanho, basta 1 deles ser subconjunto do outro */

}


int set_is_member(const Set *set, const void *data) {

    ListElmt *member;

    for (member = set->head; member != NULL; member = member ->next) {

        if ((set->match)(member->data, data) == 1) 
            return 1;

    }

    return 0;

}