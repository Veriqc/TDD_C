#ifndef NODE_HPP
#define NODE_HPP
#include "Complex.hpp"
#include "ComplexValue.hpp"
#include "Definitions.hpp"
#include "Edge.hpp"

#include <array>
#include <cstddef>
#include <utility>
#include <vector>

namespace dd {


	// NOLINTNEXTLINE(readability-identifier-naming)
	struct mNode {
		//std::array<Edge<mNode>, RADIX> e{}; // edges out of this node
		std::vector<Edge<mNode>> e; // edges out of this node
		mNode* next{};                      // used to link nodes in unique table
		uint32_t ref{};                     // reference count
		int16_t v{}; // variable index (nonterminal) value (-1 for terminal)
		std::uint8_t flags = 0;

		// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
		static mNode terminal;

		static constexpr bool isTerminal(const mNode* p) { return p == &terminal; }
		static constexpr mNode* getTerminal() { return &terminal; }

	};
	mNode mNode::terminal{{{{nullptr, Complex::zero},
                            {nullptr, Complex::zero}}},
                          nullptr,
                          0U,
                          -1,
                          32 + 16};
	using mEdge = Edge<mNode>;
	using mCachedEdge = CachedEdge<mNode>;

} // namespace dd
#endif