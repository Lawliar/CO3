// This file is part of CO3.
//
// CO3 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// CO3 is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// CO3. If not, see <https://www.gnu.org/licenses/>.


#include "ring.h"
#include <assert.h>
/**
 * @file
 * Implementation of ring buffer functions.
 */
inline uint8_t ring_buffer_is_empty(ring_buffer_t *buffer){
    return (buffer->head_index == buffer->tail_index);
}
inline uint8_t ring_buffer_is_full(ring_buffer_t *buffer) {
    return ((buffer->head_index - buffer->tail_index) & RING_BUFFER_MASK) == RING_BUFFER_MASK;
}
inline ring_buffer_size_t ring_buffer_num_items(ring_buffer_t *buffer) {
    return ((buffer->head_index - buffer->tail_index) & RING_BUFFER_MASK);
}

inline ring_buffer_size_t ring_buffer_num_empty_items(ring_buffer_t *buffer) {
    return RING_BUFFER_SIZE - ring_buffer_num_items(buffer);
}

void ring_buffer_init(ring_buffer_t *buffer) {
    buffer->tail_index = 0;
    buffer->head_index = 0;
}

void ring_buffer_queue(ring_buffer_t *buffer, char data) {
    /* Is buffer full? */
    if(ring_buffer_is_full(buffer)) {
        /* Is going to overwrite the oldest byte */
        /* Increase tail index */
        assert(0);
        buffer->tail_index = ((buffer->tail_index + 1) & RING_BUFFER_MASK);
    }

    /* Place data in buffer */
    buffer->buffer[buffer->head_index] = data;
    buffer->head_index = ((buffer->head_index + 1) & RING_BUFFER_MASK);
}

void ring_buffer_queue_arr(ring_buffer_t *buffer, const char *data, ring_buffer_size_t size) {
    /* Add bytes; one by one */
    ring_buffer_size_t i;
    for(i = 0; i < size; i++) {
        ring_buffer_queue(buffer, data[i]);
    }
}

uint8_t ring_buffer_dequeue(ring_buffer_t *buffer, char *data) {
    if(ring_buffer_is_empty(buffer)) {
        /* No items */
        return 0;
    }

    *data = buffer->buffer[buffer->tail_index];
    buffer->tail_index = ((buffer->tail_index + 1) & RING_BUFFER_MASK);
    return 1;
}

ring_buffer_size_t ring_buffer_dequeue_arr(ring_buffer_t *buffer, char *data, ring_buffer_size_t len) {
    if(ring_buffer_is_empty(buffer)) {
        /* No items */
        return 0;
    }

    char *data_ptr = data;
    ring_buffer_size_t cnt = 0;
    while((cnt < len) && ring_buffer_dequeue(buffer, data_ptr)) {
        cnt++;
        data_ptr++;
    }
    return cnt;
}

uint8_t ring_buffer_peek(ring_buffer_t *buffer, char *data, ring_buffer_size_t index) {
    if(index >= ring_buffer_num_items(buffer)) {
        /* No items at index */
        return 0;
    }

    /* Add index to pointer */
    ring_buffer_size_t data_index = ((buffer->tail_index + index) & RING_BUFFER_MASK);
    *data = buffer->buffer[data_index];
    return 1;
}
