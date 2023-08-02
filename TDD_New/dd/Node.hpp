#pragma once

#include "Complex.hpp"
#include "ComplexValue.hpp"
#include "Definitions.hpp"
#include "Edge.hpp"

#include <array>
#include <cstddef>
#include <utility>

namespace dd {


	// NOLINTNEXTLINE(readability-identifier-naming)
	struct mNode {
		std::array<Edge<mNode>, RADIX> e{}; // edges out of this node
		mNode* next{};                      // used to link nodes in unique table
		RefCount ref{};                     // reference count
		Qubit v{}; // variable index (nonterminal) value (-1 for terminal)
		std::uint8_t flags = 0;
		// 32 = marks a node with is symmetric.
		// 16 = marks a node resembling identity
		// 8 = marks a reduced dm node,
		// 4 = marks a dm (tmp flag),
		// 2 = mark first path edge (tmp flag),
		// 1 = mark path is conjugated (tmp flag))

		// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
		static mNode terminal;

		static constexpr bool isTerminal(const mNode* p) { return p == &terminal; }
		static constexpr mNode* getTerminal() { return &terminal; }

	};
	using mEdge = Edge<mNode>;
	using mCachedEdge = CachedEdge<mNode>;

} // namespace dd


