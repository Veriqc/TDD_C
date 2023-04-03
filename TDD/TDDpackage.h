/*
 * This file is part of IIC-JKU DD package which is released under the MIT license.
 * See file README.md or go to http://iic.jku.at/eda/research/quantum_dd/ for more information.
 */

#ifndef TDDpackage_H
#define TDDpackage_H

#include <unordered_set>
#include <vector>
#include <array>
#include <bitset>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <set>

#include "TDDcomplex.h"
#include<map>

using CN = dd::ComplexNumbers;

namespace dd {
	const std::string DDversion = "IIC-DD package v1.1";
	constexpr unsigned short RADIX = 2;                     // logic radix
	constexpr unsigned short NEDGE = 2;   // max no. of edges = RADIX^2

	// General package configuration parameters
	constexpr unsigned int GCLIMIT1 = 25000;                // first garbage collection limit
	constexpr unsigned int GCLIMIT_INC = 0;                  // garbage collection limit increment
	constexpr unsigned int MAXREFCNT = 20000;     // max reference count (saturates at this value)
	constexpr unsigned int NODECOUNT_BUCKETS = 10000;
	constexpr unsigned short NBUCKET = 64;                  // no. of hash table buckets; must be a power of 2
	constexpr unsigned short HASHMASK = NBUCKET - 1;  // must be nbuckets-1
	constexpr unsigned short CTSLOTS = 2048;         // no. of computed table slots
	constexpr unsigned short CTMASK = CTSLOTS - 1;    // must be CTSLOTS-1
	constexpr unsigned short TTSLOTS = 2048;          // Toffoli table slots
	constexpr unsigned short TTMASK = TTSLOTS - 1;    // must be TTSLOTS-1
	constexpr unsigned short CHUNK_SIZE = 2000;
	constexpr unsigned short MAXN = 25535;                       // max no. of inputs
	constexpr unsigned short MAXIDX = 100;   // max no. of index

    typedef struct Node *NodePtr;


	//定义指标，边，节点，TDD等
	struct Index {
		std::string key; //
		short idx;
	};

    struct Edge {
	    NodePtr p;
	    Complex w;
    };

    struct Node {
	    NodePtr next;         // link for unique table and available space chain
	    Edge e[NEDGE];     // edges out of this node
	    unsigned int ref;       // reference count
	    std::string v;        // variable index (nonterminal) value (-1 for terminal)
	    bool ident, symm; // special matrices flags
    };

	struct TDD {
		Edge e;
		std::vector<Index> index_set;
	};

    // list definitions for breadth first traversals (e.g. printing)
    typedef struct ListElement *ListElementPtr;

    struct ListElement {
	    NodePtr p;
	    ListElementPtr next;
	    int w;
    };


    // computed table definitions
    // compute table entry kinds
	enum CTkind {
		ad,
		T_ad,// TDD addition
        mult,
		con,
		slic,
        fid,
        transp,
        conjTransp,
        kron,
        noise,
        noNoise,
        none
    };

    //computed table entry
    struct CTentry1// computed table entry defn
    {
	    Edge a, b, r;     // a and b are arguments, r is the result
	    CTkind which;       // type of operation
    };

    struct CTentry2// computed table entry defn
    {
	    Edge a, b;     // a and b are arguments, r is the result
	    NodePtr r;
	    CTkind which;       // type of operation
	    ComplexValue rw;
    };

    struct CTentry3// computed table entry defn
    {
	    NodePtr a, b, r;     // a and b are arguments, r is the result
	    CTkind which;       // type of operation
	    ComplexValue aw, bw, rw;
    };

	struct CTentry4// 对应于cont
	{
		Edge a, b;
		NodePtr r;
	    std::vector<std::string> var_cont;
		ComplexValue rw;
	};

    struct TTentry // Toffoli table entry defn
    {
	    unsigned short n, m, t;
	    short line[MAXN];
	    Edge e;
    };

	enum DynamicReorderingStrategy {
		None, Sifting
	};

	enum Mode {
		Vector, Matrix, ModeCount
	};

    class Package {

    	static Node terminal;
	    constexpr static Node* terminalNode{&terminal};        // pointer to terminal node


        NodePtr nodeAvail{};                 // pointer to available space chain
	    ListElementPtr listAvail{ };           // pointer to available list elements for breadth first searches

	    // Unique Tables (one per input variable)
	    std::array<std::array<NodePtr, NBUCKET>, MAXN> Unique{ };
	    // Three types since different possibilities for complex numbers  (caused by caching)
	    // weights of operands and result are from complex table (e.g., transpose, conjugateTranspose)

		std::array<std::array<CTentry1, CTSLOTS>, static_cast<int>(Mode::ModeCount)> CTable1{};

	    // weights of operands are from complex table, weight of result from cache/ZERO (e.g., mult)
		std::array<std::array<CTentry2, CTSLOTS>, static_cast<int>(Mode::ModeCount)> CTable2{};

	    // weights of operands and result are from cache/ZERO (e.g., add)
		std::array<std::array<CTentry3, CTSLOTS>, static_cast<int>(Mode::ModeCount)> CTable3{};

		std::array<std::array<CTentry4, CTSLOTS>, static_cast<int>(Mode::ModeCount)> CTable4{};


	    // Toffoli gate table
	    std::array<TTentry, TTSLOTS> TTable{ };
	    // Identity matrix table
	    std::array<Edge, MAXN> IdTable{ };

	    unsigned int currentNodeGCLimit;              // current garbage collection limit
	    unsigned int currentComplexGCLimit;         // current complex garbage collection limit
	    std::array<int, MAXN> active{ };              // number of active nodes for each variable
	    unsigned long nodecount = 0;                // node count in unique table
	    unsigned long peaknodecount = 0;            // records peak node count in unique table

	    std::array<unsigned long, 11> nOps{};                     // operation counters
	    std::array<unsigned long, 11> CTlook{}, CThit{};      // counters for gathering compute table hit stats
        unsigned long UTcol=0, UTmatch=0, UTlookups=0;  // counter for collisions / matches in hash tables

	    std::vector<ListElementPtr> allocated_list_chunks;
	    std::vector<NodePtr> allocated_node_chunks;

		Mode mode;
	    std::unordered_set<NodePtr> visited{NODECOUNT_BUCKETS}; // 2e6

	    /// private helper routines
	    void initComputeTable();
	    NodePtr getNode();

	    Edge& UTlookup(Edge& e);

		Edge cont2(Edge x, Edge y, const std::vector<std::string>* var_cont, int var_num, int ttt=0);
		Edge cont3(Edge& x, Edge& y, const std::vector<std::string>* var_cont, int var_num);
	    static inline unsigned long CThash(const Edge& a, const Edge& b, const CTkind which) {
		    const uintptr_t node_pointer = ((uintptr_t) a.p + (uintptr_t) b.p) >> 3u;
		    const uintptr_t weights = (uintptr_t) a.w.i + (uintptr_t) a.w.r + (uintptr_t) b.w.i + (uintptr_t) b.w.r;
		    return (node_pointer + weights + (uintptr_t) which) & CTMASK;
	    }

	    static inline unsigned long CThash2(NodePtr a, const ComplexValue& aw, NodePtr b, const ComplexValue& bw, const CTkind which) {
		    const uintptr_t node_pointer = ((uintptr_t) a + (uintptr_t) b) >> 3u;
		    const uintptr_t weights = (uintptr_t) (aw.r * 1000) + (uintptr_t) (aw.i * 2000) + (uintptr_t) (bw.r * 3000) + (uintptr_t) (bw.i * 4000);
		    return (node_pointer + weights + (uintptr_t) which) & CTMASK;
	    }

	    static inline unsigned long CThash4(const Edge& a, const Edge& b, const std::vector<std::string>* var_cont) {
			const uintptr_t node_pointer = ((uintptr_t)a.p + (uintptr_t)b.p) >> 3u;
			const uintptr_t weights = (uintptr_t)a.w.i + (uintptr_t)a.w.r + (uintptr_t)b.w.i + (uintptr_t)b.w.r;

			return (node_pointer + weights+ (uintptr_t) var_cont) & CTMASK;
		}

	    static unsigned short TThash(unsigned short n, unsigned short t, const short line[]);

	    unsigned int nodeCount(const Edge& e, std::unordered_set<NodePtr>& v) const;
	    ListElementPtr newListElement();

    public:
        constexpr static Edge DDone{ terminalNode, ComplexNumbers::ONE};
        constexpr static Edge DDzero{ terminalNode, ComplexNumbers::ZERO};            // edges pointing to zero and one DD constants
        unsigned long activeNodeCount = 0;             // number of active nodes
		unsigned long maxActive = 0;
        unsigned long gc_calls{};
        unsigned long gc_runs{};
        ComplexNumbers cn;
        //std::array<unsigned short, MAXN> varOrder{ };    // variable order initially 0,1,... from bottom up | Usage: varOrder[level] := varible at a certain level
        //std::array<unsigned short, MAXN> invVarOrder{ };// inverse of variable order (inverse permutation) | Usage: invVarOrder[variable] := level of a certain variable
		std::map<std::string, int> varOrder;

        Package(std::map<std::string, int> var);
        ~Package();


        // DD creation
        static inline Edge makeTerminal(const Complex& w) { return { terminalNode, w }; }

	    Edge makeNonterminal(std::string v, const Edge *edge, bool cached = false);

	    inline Edge makeNonterminal(std::string v, const std::array<Edge, NEDGE>& edge, bool cached = false) {
	    	return makeNonterminal(v, edge.data(), cached);
	    };

        Edge CTlookup(const Edge& a, const Edge& b, CTkind which);
        void CTinsert(const Edge& a, const Edge& b, const Edge& r, CTkind which);

		Edge CTlookup4(const Edge& a, const Edge& b, const std::vector<std::string>* var_cont);
		void CTinsert4(const Edge& a, const Edge& b, const Edge& r, const std::vector<std::string>* var_cont);


		//TDD的运算
		TDD cont(TDD tdd1, TDD tdd2,int ttt=0);
		
		Edge T_add(Edge x, Edge y);
		Edge T_add2(Edge x, Edge y);
		Edge Slice(Edge x, std::string a, unsigned int c);
		TDD Matrix2TDD(const Matrix2x2& mat, std::vector<Index> var_out);
		TDD diag_matrix_2_TDD(const Matrix2x2& mat, std::vector<Index> var_out);
		TDD cnot_2_TDD(std::vector<Index> var_out, int ca);
	    Edge normalize(Edge& e, bool cached);
	    unsigned int size(const Edge& e);

	    void incRef(Edge& e);
	    void decRef(Edge& e);
		void statistics();
	    void garbageCollect(bool force = false);

	    // checks
	    static inline bool isTerminal(const Edge& e) {
		    return e.p == terminalNode;
	    }
	    static inline bool equals(const Edge& a, const Edge& b) {
		    return a.p == b.p && ComplexNumbers::equals(a.w, b.w);
	    }


		//画图
	    void toDot(Edge e, std::ostream& oss, bool isVector = false);
	    void export2Dot(Edge basic, const std::string& outputFilename, bool isVector = false, bool show = true);

	    // statistics and nodeCount
	
	    unsigned int nodeCount(const Edge& e) const {
			std::unordered_set<NodePtr> v;
			return nodeCount(e, v);
		}

	};
}
#endif
