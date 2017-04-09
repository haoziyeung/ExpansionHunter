//
// Expansion Hunter
// Copyright (c) 2016 Illumina, Inc.
//
// Author: Egor Dolzhenko <edolzhenko@illumina.com>,
//         Mitch Bekritsky <mbekritsky@illumina.com>, Richard Shaw
// Concept: Michael Eberle <meberle@illumina.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

// Defines classes and methods for genotype and haplotype likelihood
// calculations.

#pragma once

#include <array>
#include <map>
#include <string>
#include <vector>

enum class Sex { kMale, kFemale };
enum class GenotypeType { kHaploid, kDiploid };

class StrHaplotype {
public:
  StrHaplotype(int num_units_haplotype, int max_num_units_in_read,
               double prop_correct_molecules);
  double propMolecules(int num_units_upper_bound) const;
  double propMoleculesShorterThan(int num_units_upper_bound) const;
  double propMoleculesAtLeast(int num_units_lower_bound) const;
  int num_units() const { return num_units_haplotype_; }

private:
  int num_units_haplotype_;
  int max_num_units_in_read_;
  double prop_correct_molecules_;
  double norm_factor_;
  int max_deviation_;
};

class StrGenotype {
public:
  StrGenotype(int max_num_units_in_read, double prop_correct_molecules,
              double hap_depth, int read_len, int num_units_hap1,
              int num_units_hap2)
      : max_num_units_in_read_(max_num_units_in_read), hap_depth_(hap_depth),
        read_len_(read_len) {
    haplotypes.push_back(StrHaplotype(num_units_hap1, max_num_units_in_read,
                                      prop_correct_molecules));
    haplotypes.push_back(StrHaplotype(num_units_hap2, max_num_units_in_read,
                                      prop_correct_molecules));
  }
  StrGenotype(int max_num_units_in_read, double prop_correct_molecules,
              double hap_depth, int read_len, int num_units_hap)
      : hap_depth_(hap_depth), read_len_(read_len) {
    haplotypes.push_back(StrHaplotype(num_units_hap, max_num_units_in_read,
                                      prop_correct_molecules));
  }
  double calcFlankingLoglik(int num_units_in_read) const;
  double calcSpanningLoglik(int num_units_in_read) const;
  double calcLogLik(const std::map<int, int> &flanking_size_counts,
                    const std::map<int, int> &spanning_size_counts,
                    std::vector<std::array<int, 3>> &support) const;

private:
  int max_num_units_in_read_;
  double hap_depth_;
  int read_len_;
  std::vector<StrHaplotype> haplotypes;
};

std::vector<int> genotypeOneUnitStr(
    int max_num_units_in_read, double prop_correct_molecules, double hap_depth,
    int read_len, const std::vector<int> &haplotype_candidates,
    const std::map<int, int> &flanking_size_count,
    const std::map<int, int> &spanning_size_count, GenotypeType genotype_type);