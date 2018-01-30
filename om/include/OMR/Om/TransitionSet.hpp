#if !defined(OMR_OM_MEMTRANSITIONSET_HPP_)
#define OMR_OM_MEMTRANSITIONSET_HPP_

#include <OMR/Om/ArrayBuffer.hpp>
#include <OMR/Om/Id.hpp>
#include <OMR/Om/MemHandle.hpp>
#include <OMR/Om/ObjectMap.hpp>
#include <OMR/Om/SlotDescriptor.hpp>

#include <cstddef>
#include <type_traits>

namespace OMR {
namespace Om {

struct SlotMap;

/// The TransitionSet is a collection of Maps for the purpose of tracking known
/// object transitions. As objects grow slots, a chain of maps is built up. The
/// transition table tells us the existing derivations of a given map. When an
/// object grows a slot, we look to see if this layout transition has been done
/// before, so we can reuse the map. The Transition set is embedded in other
/// native objects.
class TransitionSet {
 public:
  struct Entry {
    SlotMap* map;
  };

  static bool construct(Context& cx, MemHandle<TransitionSet> self);

  TransitionSet() : table_(nullptr) {}

  std::size_t size() const { return table_->size(); }

  SlotMap* lookup(const SlotDescriptor& desc, std::size_t hash) const;

  // try to store object in the table. if the table is full, fail.
  bool tryStore(SlotMap* map, std::size_t hash);

  template <typename VisitorT>
  void visit(Context& cx, VisitorT& visitor);

 private:
  ArrayBuffer<Entry>* table_;
};

static_assert(std::is_standard_layout<TransitionSet>::value,
              "TransitionSet must be a StandardLayoutType.");

static_assert(std::is_standard_layout<TransitionSet::Entry>::value,
              "TransitionSet must be a StandardLayoutType.");

static_assert(std::is_standard_layout<ArrayBuffer<TransitionSet::Entry>>::value,
              "TransitionSet must be a StandardLayoutType.");

}  // namespace Om
}  // namespace OMR

#endif  // OMR_OM_MEMTRANSITIONSET_HPP_
  