// This file is part of SymCC.
//
// SymCC is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// SymCC is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// SymCC. If not, see <https://www.gnu.org/licenses/>.

#ifndef SHADOW_H
#define SHADOW_H
const unsigned int DR_SIZE = 4;

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iterator>
#include <map>

#include <Runtime.h>

#include <z3.h>

//
// This file is dedicated to the management of shadow memory.
//
// We manage shadows at page granularity. Since the shadow for each page is
// malloc'ed and thus at an unpredictable location in memory, we need special
// handling for memory allocations that cross page boundaries. This header
// provides iterators over shadow memory that automatically handle jumps between
// memory pages (and thus shadow regions). They should work with the C++
// standard library.
//
// We represent shadowed memory as a sequence of 8-bit expressions. The
// iterators therefore expose the shadow in the form of byte expressions.
//

constexpr uintptr_t kPageSize = 4096;


/// Compute the corresponding page address.
constexpr uintptr_t pageStart(uintptr_t addr) {
  return (addr & ~(kPageSize - 1));
}

/// Compute the corresponding offset into the page.
constexpr uintptr_t pageOffset(uintptr_t addr) {
  return (addr & (kPageSize - 1));
}

/// A mapping from page addresses to the corresponding shadow regions. Each
/// shadow is large enough to hold one expression per byte on the shadowed page.
extern std::map<uintptr_t, SymExpr *> g_shadow_pages;

/// An iterator that walks over the shadow bytes corresponding to a memory
/// region. If there is no shadow for any given memory address, it just returns
/// null.
class ReadShadowIterator
    : public std::iterator<std::bidirectional_iterator_tag, SymExpr> {
public:
  explicit ReadShadowIterator(uintptr_t address)
      : std::iterator<std::bidirectional_iterator_tag, SymExpr>(),
        address_(address), shadow_(getShadow(address)) {}

  ReadShadowIterator &operator++() {
    auto previousAddress = address_++;
    if (shadow_ != nullptr)
      shadow_++;
    if (pageStart(address_) != pageStart(previousAddress))
      shadow_ = getShadow(address_);
    return *this;
  }

  ReadShadowIterator &operator--() {
    auto previousAddress = address_--;
    if (shadow_ != nullptr)
      shadow_--;
    if (pageStart(address_) != pageStart(previousAddress))
      shadow_ = getShadow(address_);
    return *this;
  }

  SymExpr operator*() {
    assert((shadow_ == nullptr || *shadow_ == nullptr ||
            _sym_bits_helper(*shadow_) == 8) &&
           "Shadow memory always represents bytes");
    return shadow_ != nullptr ? *shadow_ : nullptr;
  }

  bool operator==(const ReadShadowIterator &other) const {
    return (address_ == other.address_);
  }

  bool operator!=(const ReadShadowIterator &other) const {
    return !(*this == other);
  }

protected:
  static SymExpr *getShadow(uintptr_t address) {
    if (auto shadowPageIt = g_shadow_pages.find(pageStart(address));
        shadowPageIt != g_shadow_pages.end())
      return shadowPageIt->second + pageOffset(address);

    return nullptr;
  }

  uintptr_t address_;
  SymExpr *shadow_;
};

/// Like ReadShadowIterator, but return an expression for the concrete memory
/// value if a region does not have a shadow.
class NonNullReadShadowIterator : public ReadShadowIterator {
public:
  explicit NonNullReadShadowIterator(uintptr_t address)
      : ReadShadowIterator(address) {}

  SymExpr operator*() {
    if (auto *symbolicResult = ReadShadowIterator::operator*())
      return symbolicResult;

    //auto int_to_build =   *reinterpret_cast<const uint8_t *>(address_);
    return _sym_build_integer(0x00, 8);
  }
};

/// An iterator that walks over the shadow corresponding to a memory region and
/// exposes it for modification. If there is no shadow yet, it creates a new
/// one.
class WriteShadowIterator : public ReadShadowIterator {
public:
  WriteShadowIterator(uintptr_t address) : ReadShadowIterator(address) {
    shadow_ = getOrCreateShadow(address);
  }

  WriteShadowIterator &operator++() {
    auto previousAddress = address_++;
    shadow_++;
    if (pageStart(address_) != pageStart(previousAddress))
      shadow_ = getOrCreateShadow(address_);
    return *this;
  }

  WriteShadowIterator &operator--() {
    auto previousAddress = address_--;
    shadow_--;
    if (pageStart(address_) != pageStart(previousAddress))
      shadow_ = getOrCreateShadow(address_);
    return *this;
  }

  SymExpr &operator*() { return *shadow_; }

protected:
  static SymExpr *getOrCreateShadow(uintptr_t address) {
    if (auto *shadow = getShadow(address))
      return shadow;

    auto *newShadow =
        static_cast<SymExpr *>(malloc(kPageSize * sizeof(SymExpr)));
    memset(newShadow, 0, kPageSize * sizeof(SymExpr));
    g_shadow_pages[pageStart(address)] = newShadow;
    return newShadow + pageOffset(address);
  }
};

class WriteShadowIteratorDR : public ReadShadowIterator {
public:
    uintptr_t orig_addr;
    uint32_t cursor;
    uint32_t len;
    WriteShadowIteratorDR(uintptr_t address, uint32_t len) : ReadShadowIterator(address), orig_addr(address),cursor(0), len(len)  {
        shadow_ = getOrCreateShadow(address);
    }

    WriteShadowIteratorDR &operator++() {
        auto previousAddress = address_++;
        if(address_ >= (orig_addr + DR_SIZE)){
            // if the new incremented address goes beyond the DR
            // reset the address and the shadow, then, fill the shadow of the DR
            assert(address_ == (orig_addr + DR_SIZE));
            address_ = orig_addr; // reset the address
            shadow_ -= DR_SIZE;         // reset the shadow

            // fill the shadow with input or null
            for(int i = 0 ; i < DR_SIZE ; i ++){
                if(cursor >= len){
                    *(shadow_ + i) = nullptr;
                }else{
                    *(shadow_ + i) = _sym_get_input_byte(cursor);
                }
                cursor++;
            }
        }else{
            shadow_++;
        }

        if (pageStart(address_) != pageStart(previousAddress))
            shadow_ = getOrCreateShadow(address_);
        return *this;
    }

    WriteShadowIteratorDR &operator--() {
        std::cerr << " you cannot read backward from a DR"
                  << std::endl;
        assert(false);
        auto previousAddress = address_--;
        shadow_--;
        if (pageStart(address_) != pageStart(previousAddress))
            shadow_ = getOrCreateShadow(address_);
        return *this;
    }

    SymExpr &operator*() { return *shadow_; }

protected:
    static SymExpr *getOrCreateShadow(uintptr_t address) {
        if (auto *shadow = getShadow(address))
            return shadow;

        auto *newShadow =
                static_cast<SymExpr *>(malloc(kPageSize * sizeof(SymExpr)));
        memset(newShadow, 0, kPageSize * sizeof(SymExpr));
        g_shadow_pages[pageStart(address)] = newShadow;
        return newShadow + pageOffset(address);
    }
};

/// A view on shadow memory that exposes read-only functionality.
struct ReadOnlyShadow {
  template <typename T>
  ReadOnlyShadow(T *addr, size_t len)
      : address_(reinterpret_cast<uintptr_t>(addr)), length_(len) {}

  ReadShadowIterator begin() const { return ReadShadowIterator(address_); }
  ReadShadowIterator end() const {
    return ReadShadowIterator(address_ + length_);
  }

  NonNullReadShadowIterator begin_non_null() const {
    return NonNullReadShadowIterator(address_);
  }

  NonNullReadShadowIterator end_non_null() const {
    return NonNullReadShadowIterator(address_ + length_);
  }

  uintptr_t address_;
  size_t length_;
};

/// A view on shadow memory that allows modifications.
template <typename T> struct ReadWriteShadow {
  ReadWriteShadow(T *addr, size_t len)
      : address_(reinterpret_cast<uintptr_t>(addr)), length_(len) {}

  WriteShadowIterator begin() { return WriteShadowIterator(address_); }
  WriteShadowIterator end() { return WriteShadowIterator(address_ + length_); }

  uintptr_t address_;
  size_t length_;
};


/// Check whether the indicated memory range is concrete, i.e., there is no
/// symbolic byte in the entire region.
template <typename T> bool isConcrete(T *addr, size_t nbytes) {
  // Fast path for allocations within one page.
  auto byteBuf = reinterpret_cast<uintptr_t>(addr);
  if (pageStart(byteBuf) == pageStart(byteBuf + nbytes) &&
      !g_shadow_pages.count(pageStart(byteBuf)))
    return true;

  ReadOnlyShadow shadow(addr, nbytes);
  return std::all_of(shadow.begin(), shadow.end(),
                     [](SymExpr expr) { return (expr == nullptr); });
}

#endif
