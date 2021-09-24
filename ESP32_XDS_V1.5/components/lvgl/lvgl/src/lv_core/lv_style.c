/**
 * @file lv_style.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_obj.h"
#include "../lv_core/lv_debug.h"
#include "../lv_misc/lv_mem.h"
#include "../lv_misc/lv_anim.h"

/*********************
 *      DEFINES
 *********************/
#define STYLE_MIX_MAX 256
#define STYLE_MIX_SHIFT 8 /*log2(STYLE_MIX_MAX)*/

#define VAL_PROP(v1, v2, r) v1 + (((v2 - v1) * r) >> STYLE_MIX_SHIFT)
#define STYLE_ATTR_MIX(attr, r)                                                                                        \
    if(start->attr != end->attr) {                                                                                     \
        res->attr = VAL_PROP(start->attr, end->attr, r);                                                               \
    } else {                                                                                                           \
        res->attr = start->attr;                                                                                       \
    }

#define LV_STYLE_PROP_TO_ID(prop) (prop & 0xFF);
#define LV_STYLE_PROP_GET_TYPE(prop) ((prop >> 8) & 0xFF);

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
LV_ATTRIBUTE_FAST_MEM static inline int32_t get_property_index(const lv_style_t * style, lv_style_property_t prop);
static lv_style_t * get_alloc_local_style(lv_style_list_t * list);

/**********************
 *  GLOABAL VARIABLES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize a style
 * @param style pointer to a style to initialize
 */
void lv_style_init(lv_style_t * style)
{
    _lv_memset_00(style, sizeof(lv_style_t));
#if LV_USE_ASSERT_STYLE
    style->sentinel = LV_DEBUG_STYLE_SENTINEL_VALUE;
#endif
}

/**
 * Copy a style with all its properties
 * @param style_dest pointer to the destination style. (Should be initialized with `lv_style_init()`)
 * @param style_src pointer to the source (to copy )style
 */
void lv_style_copy(lv_style_t * style_dest, const lv_style_t * style_src)
{
    if(style_src == NULL) return;

    LV_ASSERT_STYLE(style_dest);
    LV_ASSERT_STYLE(style_src);

    if(style_src->map == NULL) return;

    uint16_t size = _lv_style_get_mem_size(style_src);
    style_dest->map = lv_mem_alloc(size);
    _lv_memcpy(style_dest->map, style_src->map, size);
}

/**
 * Remove a property from a style
 * @param style pointer to a style
 * @param prop  a style property ORed with a state.
 * E.g. `LV_STYLE_BORDER_WIDTH | (LV_STATE_PRESSED << LV_STYLE_STATE_POS)`
 * @return true: the property was found and removed; false: the property wasn't found
 */
bool lv_style_remove_prop(lv_style_t * style, lv_style_property_t prop)
{
    if(style == NULL) return false;
    LV_ASSERT_STYLE(style);

    int32_t id = get_property_index(style, prop);
    /*The property exists but not sure it's state is the same*/
    if(id >= 0) {
        lv_style_attr_t attr_found;
        lv_style_attr_t attr_goal;

        attr_found.full = *(style->map + id + 1);
        attr_goal.full = (prop >> 8) & 0xFFU;

        if(attr_found.bits.state == attr_goal.bits.state) {
            uint32_t map_size = _lv_style_get_mem_size(style);
            uint8_t prop_size = sizeof(lv_style_property_t);
            if((prop & 0xF) < LV_STYLE_ID_COLOR) prop_size += sizeof(lv_style_int_t);
            else if((prop & 0xF) < LV_STYLE_ID_OPA) prop_size += sizeof(lv_color_t);
            else if((prop & 0xF) < LV_STYLE_ID_PTR) prop_size += sizeof(lv_opa_t);
            else prop_size += sizeof(const void *);

            /*Move the props to fill the space of the property to delete*/
            uint32_t i;
            for(i = id; i < map_size - prop_size; i++) {
                style->map[i] = style->map[i + prop_size];
            }

            style->map = lv_mem_realloc(style->map, map_size - prop_size);

            return true;
        }
    }

    return false;
}

/**
 * Initialize a style list
 * @param list a style list to initialize
 */
void lv_style_list_init(lv_style_list_t * list)
{
    _lv_memset_00(list, sizeof(lv_style_list_t));
#if LV_USE_ASSERT_STYLE
    list->sentinel = LV_DEBUG_STYLE_LIST_SENTINEL_VALUE;
#endif
}

/**
 * Copy a style list with all its styles and local style properties
 * @param list_dest pointer to the destination style list. (should be initialized with `lv_style_list_init()`)
 * @param list_src pointer to the source (to copy) style list.
 */
void lv_style_list_copy(lv_style_list_t * list_dest, const lv_style_list_t * list_src)
{
    LV_ASSERT_STYLE_LIST(list_dest);
    LV_ASSERT_STYLE_LIST(list_src);

    _lv_style_list_reset(list_dest);

    if(list_src->style_list == NULL) return;

    /*Copy the styles but skip the transitions*/
    if(list_src->has_local == 0) {
        if(list_src->has_trans) {
            list_dest->style_list = lv_mem_alloc((list_src->style_cnt - 1) * sizeof(lv_style_t *));
            _lv_memcpy(list_dest->style_list, list_src->style_list + 1, (list_src->style_cnt - 1) * sizeof(lv_style_t *));
            list_dest->style_cnt = list_src->style_cnt - 1;
        }
        else {
            list_dest->style_list = lv_mem_alloc(list_src->style_cnt * sizeof(lv_style_t *));
            _lv_memcpy(list_dest->style_list, list_src->style_list, list_src->style_cnt * sizeof(lv_style_t *));
            list_dest->style_cnt = list_src->style_cnt;
        }
    }
    else {
        if(list_src->has_trans) {
            list_dest->style_list = lv_mem_alloc((list_src->style_cnt - 2) * sizeof(lv_style_t *));
            _lv_memcpy(list_dest->style_list, list_src->style_list + 2, (list_src->style_cnt - 2) * sizeof(lv_style_t *));
            list_dest->style_cnt = list_src->style_cnt - 2;
        }
        else {
            list_dest->style_list = lv_mem_alloc((list_src->style_cnt - 1) * sizeof(lv_style_t *));
            _lv_memcpy(list_dest->style_list, list_src->style_list + 1, (list_src->style_cnt - 1) * sizeof(lv_style_t *));
            list_dest->style_cnt = list_src->style_cnt - 1;
        }

        lv_style_t * local_style = get_alloc_local_style(list_dest);
        lv_style_copy(local_style, get_alloc_local_style((lv_style_list_t *)list_src));
    }
}

/**
 * Add a style to a style list.
 * Only the the style pointer will be saved so the shouldn't be a local variable.
 * (It should be static, global or dynamically allocated)
 * @param list pointer to a style list
 * @param style pointer to a style to add
 */
void _lv_style_list_add_style(lv_style_list_t * list, lv_style_t * style)
{
    LV_ASSERT_STYLE_LIST(list);
    LV_ASSERT_STYLE(style);

    if(list == NULL) return;

    /*Remove the style first if already exists*/
    _lv_style_list_remove_style(list, style);

    lv_style_t ** new_classes;
    if(list->style_cnt == 0) new_classes = lv_mem_alloc(sizeof(lv_style_t *));
    else new_classes = lv_mem_realloc(list->style_list, sizeof(lv_style_t *) * (list->style_cnt + 1));
    LV_ASSERT_MEM(new_classes);
    if(new_classes == NULL) {
        LV_LOG_WARN("lv_style_list_add_style: couldn't add the class");
        return;
    }

    /*Make space for the new style at the beginning. Leave local and trans style if exists*/
    uint8_t i;
    uint8_t first_style = 0;
    if(list->has_trans) first_style++;
    if(list->has_local) first_style++;
    for(i = list->style_cnt; i > first_style; i--) {
        new_classes[i] = new_classes[i - 1];
    }

    new_classes[first_style] = style;
    list->style_cnt++;
    list->style_list = new_classes;
}

/**
 * Remove a style from a style list
 * @param style_list pointer to a style list
 * @param style pointer to a style to remove
 */
void _lv_style_list_remove_style(lv_style_list_t * list, lv_style_t * style)
{
    LV_ASSERT_STYLE_LIST(list);
    LV_ASSERT_STYLE(style);

    if(list->style_cnt == 0) return;

    /*Check if the style really exists here*/
    uint8_t i;
    bool found = false;
    for(i = 0; i < list->style_cnt; i++) {
        if(list->style_list[i] == style) {
            found = true;
            break;
        }
    }
    if(found == false) return;

    if(list->style_cnt == 1) {
        lv_mem_free(list->style_list);
        list->style_list = NULL;
        list->style_cnt = 0;
        list->has_local = 0;
        return;
    }

    lv_style_t ** new_classes = lv_mem_realloc(list->style_list, sizeof(lv_style_t *) * (list->style_cnt - 1));
    LV_ASSERT_MEM(new_classes);
    if(new_classes == NULL) {
        LV_LOG_WARN("lv_style_list_remove_style: couldn't reallocate class list");
        return;
    }
    uint8_t j;
    for(i = 0, j = 0; i < list->style_cnt; i++) {
        if(list->style_list[i] == style) continue;
        new_classes[j] = list->style_list[i];
        j++;

    }

    list->style_cnt--;
    list->style_list = new_classes;
}

/**
 * Remove all styles added from style list, clear the local style, transition style and free all allocated memories.
 * Leave `ignore_trans` flag as it is.
 * @param list pointer to a style list.
 */
void _lv_style_list_reset(lv_style_list_t * list)
{
    LV_ASSERT_STYLE_LIST(list);

    if(list == NULL) return;

    if(list->has_local) {
        lv_style_t * local = lv_style_list_get_local_style(list);
        if(local) {
            lv_style_reset(local);
            lv_mem_free(local);
        }
    }

    if(list->has_trans) {
        lv_style_t * trans = _lv_style_list_get_transition_style(list);
        if(trans) {
            lv_style_reset(trans);
            lv_mem_free(trans);
        }
    }

    if(list->style_cnt > 0) lv_mem_free(list->style_list);
    list->style_list = NULL;
    list->style_cnt = 0;
    list->has_local = 0;
    list->has_trans = 0;
    list->skip_trans = 0;

    /* Intentionally leave `ignore_trans` as it is,
     * because it's independent from the styles in the list*/
}

/**
 * Clear all properties from a style and all allocated memories.
 * @param style pointer to a style
 */
void lv_style_reset(lv_style_t * style)
{
    LV_ASSERT_STYLE(style);

    lv_mem_free(style->map);
    style->map = NULL;
}

/**
 * Get the size of the properties in a style in bytes
 * @param style pointer to a style
 * @return size of the properties in bytes
 */
uint16_t _lv_style_get_mem_size(const lv_style_t * style)
{
    LV_ASSERT_STYLE(style);

    if(style->map == NULL) return 0;

    size_t i = 0;
    while(style->map[i] != _LV_STYLE_CLOSEING_PROP) {
        /*Go to the next property*/
        if((style->map[i] & 0xF) < LV_STYLE_ID_COLOR) i += sizeof(lv_style_int_t);
        else if((style->map[i] & 0xF) < LV_STYLE_ID_OPA) i += sizeof(lv_color_t);
        else if((style->map[i] & 0xF) < LV_STYLE_ID_PTR) i += sizeof(lv_opa_t);
        else i += sizeof(const void *);

        i += sizeof(lv_style_property_t);
    }

    return i + sizeof(lv_style_property_t);
}

/**
 * Set an integer typed property in a style.
 * @param style pointer to a style where the property should be set
 * @param prop a style property ORed with a state.
 * E.g. `LV_STYLE_BORDER_WIDTH | (LV_STATE_PRESSED << LV_STYLE_STATE_POS)`
 * @param value the value to set
 * @note shouldn't be used directly. Use the specific property set functions instead.
 *       For example: `lv_style_set_border_width()`
 * @note for performance reasons it's not checked if the property really has integer type
 */
void _lv_style_set_int(lv_style_t * style, lv_style_property_t prop, lv_style_int_t value)
{
    LV_ASSERT_STYLE(style);

    int32_t id = get_property_index(style, prop);
    /*The property already exists but not sure it's state is the same*/
    if(id >= 0) {
        lv_style_attr_t attr_found;
        lv_style_attr_t attr_goal;

        attr_found.full = *(style->map + id + 1);
        attr_goal.full = (prop >> 8) & 0xFFU;

        if(attr_found.bits.state == attr_goal.bits.state) {
            _lv_memcpy_small(style->map + id + sizeof(lv_style_property_t), &value, sizeof(lv_style_int_t));
            return;
        }
    }

    /*Add new property if not exists yet*/
    uint8_t new_prop_size = (sizeof(lv_style_property_t) + sizeof(lv_style_int_t));
    lv_style_property_t end_mark = _LV_STYLE_CLOSEING_PROP;
    uint8_t end_mark_size = sizeof(end_mark);

    uint16_t size = _lv_style_get_mem_size(style);
    if(size == 0) size += end_mark_size;
    size += sizeof(lv_style_property_t) + sizeof(lv_style_int_t);
    style->map = lv_mem_realloc(style->map, size);
    LV_ASSERT_MEM(style->map);
    if(style == NULL) return;

    _lv_memcpy_small(style->map + size - new_prop_size - end_mark_size, &prop, sizeof(lv_style_property_t));
    _lv_memcpy_small(style->map + size - sizeof(lv_style_int_t) - end_mark_size, &value, sizeof(lv_style_int_t));
    _lv_memcpy_small(style->map + size - end_mark_size, &end_mark, sizeof(end_mark));
}

/**
 * Set a color typed property in a style.
 * @param style pointer to a style where the property should be set
 * @param prop a style property ORed with a state.
 * E.g. `LV_STYLE_BORDER_COLOR | (LV_STATE_PRESSED << LV_STYLE_STATE_POS)`
 * @param value the value to set
 * @note shouldn't be used directly. Use the specific property set functions instead.
 *       For example: `lv_style_set_border_color()`
 * @note for performance reasons it's not checked if the property really has color type
 */
/**
*在样式中设置颜色类型属性。
*@param style指向应设置属性的样式的指针
*@param prop一个带有状态的样式属性。
*例如` LV U STYLE_BORDER|颜色|（LV U STATE_Presed<<LV\u STYLE_STATE\u POS）`
*@param value要设置的值
*@note不应该直接使用。请改用特定的属性集函数。
*例如：`lv\U样式\u set\u border\u color（）`
*@注意性能原因属性是否确实具有颜色类型，则未检查
*/
void _lv_style_set_color(lv_style_t * style, lv_style_property_t prop, lv_color_t color)
{
    LV_ASSERT_STYLE(style);

    int32_t id = get_property_index(style, prop);
    /*The property already exists but not sure it's state is the same*/
    if(id >= 0) {
        lv_style_attr_t attr_found;
        lv_style_attr_t attr_goal;

        attr_found.full = *(style->map + id + 1);
        attr_goal.full = (prop >> 8) & 0xFFU;

        if(attr_found.bits.state == attr_goal.bits.state) {
            _lv_memcpy_small(style->map + id + sizeof(lv_style_property_t), &color, sizeof(lv_color_t));
            return;
        }
    }

    /*Add new property if not exists yet*/
    uint8_t new_prop_size = (sizeof(lv_style_property_t) + sizeof(lv_color_t));
    lv_style_property_t end_mark = _LV_STYLE_CLOSEING_PROP;
    uint8_t end_mark_size = sizeof(end_mark);

    uint16_t size = _lv_style_get_mem_size(style);
    if(size == 0) size += end_mark_size;

    size += sizeof(lv_style_property_t) + sizeof(lv_color_t);
    style->map = lv_mem_realloc(style->map, size);
    LV_ASSERT_MEM(style->map);
    if(style == NULL) return;

    _lv_memcpy_small(style->map + size - new_prop_size - end_mark_size, &prop, sizeof(lv_style_property_t));
    _lv_memcpy_small(style->map + size - sizeof(lv_color_t) - end_mark_size, &color, sizeof(lv_color_t));
    _lv_memcpy_small(style->map + size - end_mark_size, &end_mark, sizeof(end_mark));
}

/**
 * Set an opacity typed property in a style.
 * @param style pointer to a style where the property should be set
 * @param prop a style property ORed with a state.
 * E.g. `LV_STYLE_BORDER_OPA | (LV_STATE_PRESSED << LV_STYLE_STATE_POS)`
 * @param value the value to set
 * @note shouldn't be used directly. Use the specific property set functions instead.
 *       For example: `lv_style_set_border_opa()`
 * @note for performance reasons it's not checked if the property really has opacity type
 */
void _lv_style_set_opa(lv_style_t * style, lv_style_property_t prop, lv_opa_t opa)
{
    LV_ASSERT_STYLE(style);

    int32_t id = get_property_index(style, prop);
    /*The property already exists but not sure it's state is the same*/
    if(id >= 0) {
        lv_style_attr_t attr_found;
        lv_style_attr_t attr_goal;

        attr_found.full = *(style->map + id + 1);
        attr_goal.full = (prop >> 8) & 0xFFU;

        if(attr_found.bits.state == attr_goal.bits.state) {
            _lv_memcpy_small(style->map + id + sizeof(lv_style_property_t), &opa, sizeof(lv_opa_t));
            return;
        }
    }

    /*Add new property if not exists yet*/
    uint8_t new_prop_size = (sizeof(lv_style_property_t) + sizeof(lv_opa_t));
    lv_style_property_t end_mark = _LV_STYLE_CLOSEING_PROP;
    uint8_t end_mark_size = sizeof(end_mark);

    uint16_t size = _lv_style_get_mem_size(style);
    if(size == 0) size += end_mark_size;

    size += sizeof(lv_style_property_t) + sizeof(lv_opa_t);
    style->map = lv_mem_realloc(style->map, size);
    LV_ASSERT_MEM(style->map);
    if(style == NULL) return;

    _lv_memcpy_small(style->map + size - new_prop_size - end_mark_size, &prop, sizeof(lv_style_property_t));
    _lv_memcpy_small(style->map + size - sizeof(lv_opa_t) - end_mark_size, &opa, sizeof(lv_opa_t));
    _lv_memcpy_small(style->map + size - end_mark_size, &end_mark, sizeof(end_mark));
}

/**
 * Set a pointer typed property in a style.
 * @param style pointer to a style where the property should be set
 * @param prop a style property ORed with a state.
 * E.g. `LV_STYLE_TEXT_POINTER | (LV_STATE_PRESSED << LV_STYLE_STATE_POS)`
 * @param value the value to set
 * @note shouldn't be used directly. Use the specific property set functions instead.
 *       For example: `lv_style_set_border_width()`
 * @note for performance reasons it's not checked if the property is really has pointer type
 */
void _lv_style_set_ptr(lv_style_t * style, lv_style_property_t prop, const void * p)
{
    LV_ASSERT_STYLE(style);

    int32_t id = get_property_index(style, prop);
    /*The property already exists but not sure it's state is the same*/
    if(id >= 0) {
        lv_style_attr_t attr_found;
        lv_style_attr_t attr_goal;

        attr_found.full = *(style->map + id + 1);
        attr_goal.full = (prop >> 8) & 0xFFU;

        if(attr_found.bits.state == attr_goal.bits.state) {
            _lv_memcpy_small(style->map + id + sizeof(lv_style_property_t), &p, sizeof(const void *));
            return;
        }
    }

    /*Add new property if not exists yet*/
    uint8_t new_prop_size = (sizeof(lv_style_property_t) + sizeof(const void *));
    lv_style_property_t end_mark = _LV_STYLE_CLOSEING_PROP;
    uint8_t end_mark_size = sizeof(end_mark);

    uint16_t size = _lv_style_get_mem_size(style);
    if(size == 0) size += end_mark_size;

    size += sizeof(lv_style_property_t) + sizeof(const void *);
    style->map = lv_mem_realloc(style->map, size);
    LV_ASSERT_MEM(style->map);
    if(style == NULL) return;

    _lv_memcpy_small(style->map + size - new_prop_size - end_mark_size, &prop, sizeof(lv_style_property_t));
    _lv_memcpy_small(style->map + size - sizeof(const void *) - end_mark_size, &p, sizeof(const void *));
    _lv_memcpy_small(style->map + size - end_mark_size, &end_mark, sizeof(end_mark));
}

/**
 * Get the a property from a style.
 * Take into account the style state and return the property which matches the best.
 * @param style pointer to a style where to search
 * @param prop the property, might contain ORed style states too
 * @param res buffer to store the result
 * @return the weight of the found property (how well it fits to the style state).
 *         Higher number is means better fit
 *         -1 if the not found (`res` will be undefined)
 */
int16_t _lv_style_get_int(const lv_style_t * style, lv_style_property_t prop, void * v_res)
{
    lv_style_int_t * res = (lv_style_int_t *)v_res;
    LV_ASSERT_STYLE(style);

    if(style == NULL) return -1;
    if(style->map == NULL) return -1;

    int32_t id = get_property_index(style, prop);
    if(id < 0) {
        return -1;
    }
    else {
        _lv_memcpy_small(res, &style->map[id + sizeof(lv_style_property_t)], sizeof(lv_style_int_t));
        lv_style_attr_t attr_act;
        attr_act.full = style->map[id + 1];

        lv_style_attr_t attr_goal;
        attr_goal.full = (prop >> 8) & 0xFF;

        return attr_act.bits.state & attr_goal.bits.state;
    }
}

/**
 * Get an opacity typed property from a style.
 * @param style pointer to a style from where the property should be get
 * @param prop a style property ORed with a state.
 * E.g. `LV_STYLE_BORDER_OPA | (LV_STATE_PRESSED << LV_STYLE_STATE_POS)`
 * @param res pointer to a buffer to store the result value
 * @return -1: the property wasn't found in the style.
 *         The matching state bits of the desired state (in `prop`) and the best matching property's state
 *         Higher value means match in higher precedence state.
 * @note shouldn't be used directly. Use the specific property get functions instead.
 *       For example: `lv_style_get_border_opa()`
 * @note for performance reasons it's not checked if the property really has opacity type
 */
int16_t _lv_style_get_opa(const lv_style_t * style, lv_style_property_t prop, void * v_res)
{
    lv_opa_t * res = (lv_opa_t *)v_res;
    LV_ASSERT_STYLE(style);

    if(style == NULL) return -1;
    if(style->map == NULL) return -1;

    int32_t id = get_property_index(style, prop);
    if(id < 0) {
        return -1;
    }
    else {
        _lv_memcpy_small(res, &style->map[id + sizeof(lv_style_property_t)], sizeof(lv_opa_t));
        lv_style_attr_t attr_act;
        attr_act.full = style->map[id + 1];

        lv_style_attr_t attr_goal;
        attr_goal.full = (prop >> 8) & 0xFF;

        return attr_act.bits.state & attr_goal.bits.state;
    }
}

/**
 * Get a color typed property from a style.
 * @param style pointer to a style from where the property should be get
 * @param prop a style property ORed with a state.
 * E.g. `LV_STYLE_BORDER_COLOR | (LV_STATE_PRESSED << LV_STYLE_STATE_POS)`
 * @param res pointer to a buffer to store the result value
 * @return -1: the property wasn't found in the style.
 *         The matching state bits of the desired state (in `prop`) and the best matching property's state
 *         Higher value means match in higher precedence state.
 * @note shouldn't be used directly. Use the specific property get functions instead.
 *       For example: `lv_style_get_border_color()`
 * @note for performance reasons it's not checked if the property really has color type
 */
int16_t _lv_style_get_color(const lv_style_t * style, lv_style_property_t prop, void * v_res)
{
    lv_color_t * res = (lv_color_t *)v_res;
    if(style == NULL) return -1;
    if(style->map == NULL) return -1;
    int32_t id = get_property_index(style, prop);
    if(id < 0) {
        return -1;
    }
    else {
        _lv_memcpy_small(res, &style->map[id + sizeof(lv_style_property_t)], sizeof(lv_color_t));
        lv_style_attr_t attr_act;
        attr_act.full = style->map[id + 1];

        lv_style_attr_t attr_goal;
        attr_goal.full = (prop >> 8) & 0xFF;

        return attr_act.bits.state & attr_goal.bits.state;
    }
}

/**
 * Get a pointer typed property from a style.
 * @param style pointer to a style from where the property should be get
 * @param prop a style property ORed with a state.
 * E.g. `LV_STYLE_TEXT_FONT | (LV_STATE_PRESSED << LV_STYLE_STATE_POS)`
 * @param res pointer to a buffer to store the result value
 * @return -1: the property wasn't found in the style.
 *         The matching state bits of the desired state (in `prop`) and the best matching property's state
 *         Higher value means match in higher precedence state.
 * @note shouldn't be used directly. Use the specific property get functions instead.
 *       For example: `lv_style_get_text_font()`
 * @note for performance reasons it's not checked if the property really has pointer type
 */
int16_t _lv_style_get_ptr(const lv_style_t * style, lv_style_property_t prop, void * v_res)
{
    const void ** res = (const void **)v_res;
    if(style == NULL) return -1;
    if(style->map == NULL) return -1;

    int32_t id = get_property_index(style, prop);
    if(id < 0) {
        return -1;
    }
    else {
        _lv_memcpy_small(res, &style->map[id + sizeof(lv_style_property_t)], sizeof(const void *));
        lv_style_attr_t attr_act;
        attr_act.full = style->map[id + 1];

        lv_style_attr_t attr_goal;
        attr_goal.full = (prop >> 8) & 0xFF;

        return attr_act.bits.state & attr_goal.bits.state;
    }
}

/**
 * Get the local style of a style list
 * @param list pointer to a style list where the local property should be set
 * @return pointer to the local style if exists else `NULL`.
 */
lv_style_t * lv_style_list_get_local_style(lv_style_list_t * list)
{
    LV_ASSERT_STYLE_LIST(list);

    if(!list->has_local) return NULL;
    if(list->has_trans) return list->style_list[1];
    else return list->style_list[0];
}

/**
 * Get the transition style of a style list
 * @param list pointer to a style list where the local property should be set
 * @return pointer to the transition style if exists else `NULL`.
 */
lv_style_t * _lv_style_list_get_transition_style(lv_style_list_t * list)
{
    LV_ASSERT_STYLE_LIST(list);

    if(!list->has_trans) return NULL;
    return list->style_list[0];
}

/**
 * Allocate the transition style in a style list. If already exists simply return it.
 * @param list pointer to a style list
 * @return the transition style of a style list
 */
lv_style_t * _lv_style_list_add_trans_style(lv_style_list_t * list)
{
    LV_ASSERT_STYLE_LIST(list);
    if(list->has_trans) return _lv_style_list_get_transition_style(list);

    lv_style_t * trans_style = lv_mem_alloc(sizeof(lv_style_t));
    LV_ASSERT_MEM(trans_style);
    if(trans_style == NULL) {
        LV_LOG_WARN("lv_style_list_add_trans_style: couldn't create transition style");
        return NULL;
    }

    lv_style_init(trans_style);

    _lv_style_list_add_style(list, trans_style);
    list->has_trans = 1;

    /*If the list has local style trans was added after it. But trans should be the first so swap them*/
    if(list->has_local) {
        lv_style_t * tmp = list->style_list[0];
        list->style_list[0] = list->style_list[1];
        list->style_list[1] = tmp;
    }
    return trans_style;
}


/**
 * Set a local integer typed property in a style list.
 * @param list pointer to a style list where the local property should be set
 * @param prop a style property ORed with a state.
 * E.g. `LV_STYLE_BORDER_WIDTH | (LV_STATE_PRESSED << LV_STYLE_STATE_POS)`
 * @param value the value to set
 * @note for performance reasons it's not checked if the property really has integer type
 */
void _lv_style_list_set_local_int(lv_style_list_t * list, lv_style_property_t prop, lv_style_int_t value)
{
    LV_ASSERT_STYLE_LIST(list);

    lv_style_t * local = get_alloc_local_style(list);
    _lv_style_set_int(local, prop, value);
}

/**
 * Set a local opacity typed property in a style list.
 * @param list pointer to a style list where the local property should be set
 * @param prop a style property ORed with a state.
 * E.g. `LV_STYLE_BORDER_OPA | (LV_STATE_PRESSED << LV_STYLE_STATE_POS)`
 * @param value the value to set
 * @note for performance reasons it's not checked if the property really has opacity type
 */
void _lv_style_list_set_local_opa(lv_style_list_t * list, lv_style_property_t prop, lv_opa_t value)
{
    LV_ASSERT_STYLE_LIST(list);

    lv_style_t * local = get_alloc_local_style(list);
    _lv_style_set_opa(local, prop, value);
}

/**
 * Set a local color typed property in a style list.
 * @param list pointer to a style list where the local property should be set
 * @param prop a style property ORed with a state.
 * E.g. `LV_STYLE_BORDER_COLOR | (LV_STATE_PRESSED << LV_STYLE_STATE_POS)`
 * @param value the value to set
 * @note for performance reasons it's not checked if the property really has color type
 */
void _lv_style_list_set_local_color(lv_style_list_t * list, lv_style_property_t prop, lv_color_t value)
{
    LV_ASSERT_STYLE_LIST(list);

    lv_style_t * local = get_alloc_local_style(list);
    _lv_style_set_color(local, prop, value);
}

/**
 * Set a local pointer typed property in a style list.
 * @param list pointer to a style list where the local property should be set
 * @param prop a style property ORed with a state.
 * E.g. `LV_STYLE_TEXT_FONT | (LV_STATE_PRESSED << LV_STYLE_STATE_POS)`
 * @param value the value to set
 * @note for performance reasons it's not checked if the property really has pointer type
 */
void _lv_style_list_set_local_ptr(lv_style_list_t * list, lv_style_property_t prop, const void * value)
{
    LV_ASSERT_STYLE_LIST(list);

    lv_style_t * local = get_alloc_local_style(list);
    _lv_style_set_ptr(local, prop, value);
}



/**
 * Get an integer typed property from a style list.
 * It will return the property which match best with given state.
 * @param list pointer to a style list from where the property should be get
 * @param prop a style property ORed with a state.
 * E.g. `LV_STYLE_BORDER_WIDTH | (LV_STATE_PRESSED << LV_STYLE_STATE_POS)`
 * @param res pointer to a buffer to store the result
 * @return LV_RES_OK: there was a matching property in the list
 *         LV_RES_INV: there was NO matching property in the list
 * @note for performance reasons it's not checked if the property really has integer type
 */
lv_res_t _lv_style_list_get_int(lv_style_list_t * list, lv_style_property_t prop, lv_style_int_t * res)
{
    LV_ASSERT_STYLE_LIST(list);

    if(list == NULL) return LV_RES_INV;
    if(list->style_list == NULL) return LV_RES_INV;

    lv_style_attr_t attr;
    attr.full = prop >> 8;
    int16_t weight_goal = attr.full;

    int16_t weight = -1;

    lv_style_int_t value_act = 0;

    int16_t ci;
    for(ci = 0; ci < list->style_cnt; ci++) {
        lv_style_t * class = lv_style_list_get_style(list, ci);
        int16_t weight_act = _lv_style_get_int(class, prop, &value_act);

        /*On perfect match return the value immediately*/
        if(weight_act == weight_goal) {
            *res = value_act;
            return LV_RES_OK;
        }
        else if(list->has_trans && weight_act >= 0 && ci == 0 && !list->skip_trans) {
            *res = value_act;
            return LV_RES_OK;
        }
        /*If the found ID is better the current candidate then use it*/
        else if(weight_act > weight) {
            weight =  weight_act;
            *res = value_act;
        }
    }

    if(weight >= 0) return LV_RES_OK;
    else return LV_RES_INV;

}

/**
 * Get a color typed property from a style list.
 * It will return the property which match best with given state.
 * @param list pointer to a style list from where the property should be get
 * @param prop a style property ORed with a state.
 * E.g. `LV_STYLE_BORDER_COLOR | (LV_STATE_PRESSED << LV_STYLE_STATE_POS)`
 * @param res pointer to a buffer to store the result
 * @return LV_RES_OK: there was a matching property in the list
 *         LV_RES_INV: there was NO matching property in the list
 * @note for performance reasons it's not checked if the property really has color type
 */
lv_res_t _lv_style_list_get_color(lv_style_list_t * list, lv_style_property_t prop, lv_color_t * res)
{
    LV_ASSERT_STYLE_LIST(list);

    if(list == NULL) return LV_RES_INV;
    if(list->style_list == NULL) return LV_RES_INV;

    lv_style_attr_t attr;
    attr.full = prop >> 8;
    int16_t weight_goal = attr.full;

    int16_t weight = -1;

    lv_color_t value_act = { 0 };

    int16_t ci;
    for(ci = 0; ci < list->style_cnt; ci++) {
        lv_style_t * class = lv_style_list_get_style(list, ci);
        int16_t weight_act = _lv_style_get_color(class, prop, &value_act);
        /*On perfect match return the value immediately*/
        if(weight_act == weight_goal) {
            *res = value_act;
            return LV_RES_OK;
        }
        else if(list->has_trans && weight_act >= 0 && ci == 0 && !list->skip_trans) {
            *res = value_act;
            return LV_RES_OK;
        }
        /*If the found ID is better the current candidate then use it*/
        else if(weight_act > weight) {
            weight =  weight_act;
            *res = value_act;
        }
    }

    if(weight >= 0)  return LV_RES_OK;
    else return LV_RES_INV;
}

/**
 * Get an opacity typed property from a style list.
 * It will return the property which match best with given state.
 * @param list pointer to a style list from where the property should be get
 * @param prop a style property ORed with a state.
 * E.g. `LV_STYLE_BORDER_OPA| (LV_STATE_PRESSED << LV_STYLE_STATE_POS)`
 * @param res pointer to a buffer to store the result
 * @return LV_RES_OK: there was a matching property in the list
 *         LV_RES_INV: there was NO matching property in the list
 * @note for performance reasons it's not checked if the property really has opacity type
 */
lv_res_t _lv_style_list_get_opa(lv_style_list_t * list, lv_style_property_t prop, lv_opa_t * res)
{
    LV_ASSERT_STYLE_LIST(list);

    if(list == NULL) return LV_RES_INV;
    if(list->style_list == NULL) return LV_RES_INV;

    lv_style_attr_t attr;
    attr.full = prop >> 8;
    int16_t weight_goal = attr.full;

    int16_t weight = -1;

    lv_opa_t value_act = LV_OPA_TRANSP;

    int16_t ci;
    for(ci = 0; ci < list->style_cnt; ci++) {
        lv_style_t * class = lv_style_list_get_style(list, ci);
        int16_t weight_act = _lv_style_get_opa(class, prop, &value_act);
        /*On perfect match return the value immediately*/
        if(weight_act == weight_goal) {
            *res = value_act;
            return LV_RES_OK;
        }
        else if(list->has_trans && weight_act >= 0 && ci == 0 && !list->skip_trans) {
            *res = value_act;
            return LV_RES_OK;
        }
        /*If the found ID is better the current candidate then use it*/
        else if(weight_act > weight) {
            weight =  weight_act;
            *res = value_act;
        }
    }

    if(weight >= 0)  return LV_RES_OK;
    else return LV_RES_INV;
}

/**
 * Get a pointer typed property from a style list.
 * It will return the property which match best with given state.
 * @param list pointer to a style list from where the property should be get
 * @param prop a style property ORed with a state.
 * E.g. `LV_STYLE_TEXT_FONT | (LV_STATE_PRESSED << LV_STYLE_STATE_POS)`
 * @param res pointer to a buffer to store the result
 * @return LV_RES_OK: there was a matching property in the list
 *         LV_RES_INV: there was NO matching property in the list
 * @note for performance reasons it's not checked if the property really has pointer type
 */
lv_res_t _lv_style_list_get_ptr(lv_style_list_t * list, lv_style_property_t prop, const void ** res)
{
    LV_ASSERT_STYLE_LIST(list);

    if(list == NULL) return LV_RES_INV;
    if(list->style_list == NULL) return LV_RES_INV;

    lv_style_attr_t attr;
    attr.full = prop >> 8;
    int16_t weight_goal = attr.full;

    int16_t weight = -1;

    const void * value_act;

    int16_t ci;
    for(ci = 0; ci < list->style_cnt; ci++) {
        lv_style_t * class = lv_style_list_get_style(list, ci);
        int16_t weight_act = _lv_style_get_ptr(class, prop, &value_act);
        /*On perfect match return the value immediately*/
        if(weight_act == weight_goal) {
            *res = value_act;
            return LV_RES_OK;
        }
        else if(list->has_trans && weight_act >= 0 && ci == 0 && !list->skip_trans) {
            *res = value_act;
            return LV_RES_OK;
        }
        /*If the found ID is better the current candidate then use it*/
        else if(weight_act > weight) {
            weight =  weight_act;
            *res = value_act;
        }
    }

    if(weight >= 0)  return LV_RES_OK;
    else return LV_RES_INV;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Get a property's index (byte index in `style->map`) from a style.
 * Return best matching property's index considering the state of `prop`
 * @param style pointer to a style
 * @param prop a style property ORed with a state.
 * E.g. `LV_STYLE_TEXT_FONT | (LV_STATE_PRESSED << LV_STYLE_STATE_POS)`
 * @return
 */
LV_ATTRIBUTE_FAST_MEM static inline int32_t get_property_index(const lv_style_t * style, lv_style_property_t prop)
{
    LV_ASSERT_STYLE(style);

    if(style->map == NULL) return -1;

    uint8_t id_to_find = prop & 0xFF;
    lv_style_attr_t attr;
    attr.full = (prop >> 8) & 0xFF;

    int16_t weight = -1;
    int16_t id_guess = -1;

    size_t i = 0;
    while(style->map[i] != _LV_STYLE_CLOSEING_PROP) {
        if(style->map[i] == id_to_find) {
            lv_style_attr_t attr_i;
            attr_i.full = style->map[i + 1];

            /*If the state perfectly matches return this property*/
            if(attr_i.bits.state == attr.bits.state) {
                return i;
            }
            /* Be sure the property not specifies other state than the requested.
             * E.g. For HOVER+PRESS, HOVER only is OK, but HOVER+FOCUS not*/
            else if((attr_i.bits.state & (~attr.bits.state)) == 0) {
                /* Use this property if it describes better the requested state than the current candidate.
                 * E.g. for HOVER+FOCUS+PRESS prefer HOVER+FOCUS over FOCUS*/
                if(attr_i.bits.state > weight) {
                    weight = attr_i.bits.state;
                    id_guess = i;
                }
            }
        }

        /*Go to the next property*/
        if((style->map[i] & 0xF) < LV_STYLE_ID_COLOR) i += sizeof(lv_style_int_t);
        else if((style->map[i] & 0xF) < LV_STYLE_ID_OPA) i += sizeof(lv_color_t);
        else if((style->map[i] & 0xF) < LV_STYLE_ID_PTR) i += sizeof(lv_opa_t);
        else i += sizeof(const void *);

        i += sizeof(lv_style_property_t);
    }

    return id_guess;
}

/**
 * Get he local style from a style list. Allocate it if not exists yet.
 * @param list pointer to a style list
 * @return pointer to the local style
 */
static lv_style_t * get_alloc_local_style(lv_style_list_t * list)
{
    LV_ASSERT_STYLE_LIST(list);

    if(list->has_local) return lv_style_list_get_style(list, 0);

    lv_style_t * local_style = lv_mem_alloc(sizeof(lv_style_t));
    LV_ASSERT_MEM(local_style);
    if(local_style == NULL) {
        LV_LOG_WARN("get_local_style: couldn't create local style");
        return NULL;
    }
    lv_style_init(local_style);

    /*Add the local style to the furst place*/
    _lv_style_list_add_style(list, local_style);
    list->has_local = 1;

    return local_style;
}
