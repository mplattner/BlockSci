//
//  pubkey_range_methods.cpp
//  blocksci
//
//  Created by Harry Kalodner on 4/11/18.
//
//

#include "pubkey_range_py.hpp"
#include "pubkey_py.hpp"
#include "scripts/address_py.hpp"
#include "caster_py.hpp"
#include "range_apply_py.hpp"

using namespace blocksci;
namespace py = pybind11;

void applyMethodsToPubkeyRange(RangeClasses<script::Pubkey> &classes) {
	applyMethodsToRange(classes, AddAddressMethods<ScriptBase>{});
    applyMethodsToRange(classes, AddPubkeyBaseMethods<script::Pubkey>{});
}
