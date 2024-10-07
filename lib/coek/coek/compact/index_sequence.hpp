#pragma once

#include <memory>
#include <vector>

namespace coek {

class IndexSeqIteratorRepn;
class IndexSequenceRepn;
class SequenceContext;
class IndexParameter;

class IndexSeqIterator {
   public:
    std::shared_ptr<IndexSeqIteratorRepn> repn;

    typedef std::vector<IndexParameter>* pointer;
    typedef const std::vector<IndexParameter>* const_pointer;
    typedef std::vector<IndexParameter>& reference;
    typedef const std::vector<IndexParameter>& const_reference;

   public:
    IndexSeqIterator();
    IndexSeqIterator(IndexSequenceRepn* seq, bool end = false);

    IndexSeqIterator& operator++();

    bool operator==(const IndexSeqIterator& other) const;
    bool operator!=(const IndexSeqIterator& other) const;

    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;
};

class IndexSequence {
   public:
    std::shared_ptr<IndexSequenceRepn> repn;

   public:
    IndexSequence(const std::shared_ptr<IndexSequenceRepn>& _repn);
    IndexSequence(const SequenceContext& context);

    IndexSeqIterator begin();
    IndexSeqIterator end();
    const IndexSeqIterator begin() const;
    const IndexSeqIterator end() const;
};

}  // namespace coek
