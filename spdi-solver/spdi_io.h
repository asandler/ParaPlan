#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "succ.h"
#include "helpers.h"

using namespace std;

void ReadAndValidateSPDI(const string& filename, SPDI& spdi);

void ReadStartAndFinalEdgeParts(const string& filename, const SPDI& spdi, SPDIReachTask& reachTask);
