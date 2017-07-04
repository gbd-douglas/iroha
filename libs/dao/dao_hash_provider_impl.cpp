/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dao_hash_provider_impl.hpp"

namespace iroha {
  namespace dao {

    iroha::hash256_t HashProviderImpl::get_hash(const Proposal &proposal) {
      std::string concat_;  // string representation of the proposal, made of
      // proposals meta and body fields
      for (auto tx : proposal.transactions) {
        for (auto command : tx.commands) {
          std::array<char, sizeof(*command)> command_blob;
          std::copy_n((char *)command.get(), sizeof(*command), command_blob.begin());
          std::copy(command_blob.begin(), command_blob.end(),
                    std::back_inserter(concat_));
        }
        std::copy(tx.creator.begin(), tx.creator.end(),
                  std::back_inserter(concat_));
      }
      std::vector<uint8_t> concat(concat_.begin(), concat_.end());

      auto concat_hash = sha3_256(concat.data(), concat.size());
      return concat_hash;
    }

    iroha::hash256_t HashProviderImpl::get_hash(const Block &block) {
      std::string concat_;

      // block height
      concat_ += std::to_string(block.height);

      // prev_hash
      std::copy(block.prev_hash.begin(), block.prev_hash.end(),
                std::back_inserter(concat_));

      // txnumber
      concat_ += std::to_string(block.txs_number);

      // merkle root
      std::copy(block.merkle_root.begin(), block.merkle_root.end(),
                std::back_inserter(concat_));

      // transactions
      for (auto tx : block.transactions) {
        for (auto command : tx.commands) {
          std::array<char, sizeof(*command)> command_blob;
          std::copy_n((char *)command.get(), sizeof(*command), command_blob.begin());
          std::copy(command_blob.begin(), command_blob.end(),
                    std::back_inserter(concat_));
        }
        std::copy(tx.creator.begin(), tx.creator.end(),
                  std::back_inserter(concat_));

        concat_ += std::to_string(tx.created_ts);

        concat_ += std::to_string(tx.tx_counter);

        for (auto sig : tx.signatures) {
          std::copy(sig.pubkey.begin(), sig.pubkey.end(),
                    std::back_inserter(concat_));
          std::copy(sig.signature.begin(), sig.signature.end(),
                    std::back_inserter(concat_));
        }
      }
      std::vector<uint8_t> concat(concat_.begin(), concat_.end());

      auto concat_hash = sha3_256(concat.data(), concat.size());
      return concat_hash;
    }

    iroha::hash256_t HashProviderImpl::get_hash(const Transaction &tx) {
      std::string concat_hash_commands_;
      for (auto command : tx.commands) {
        // convert command to blob and concat it to result string
        std::array<char, sizeof(*command)> command_blob;
        std::copy_n((char *)command.get(), sizeof(*command), command_blob.begin());
        std::copy(command_blob.begin(), command_blob.end(),
                  std::back_inserter(concat_hash_commands_));
      }
      std::copy(tx.creator.begin(), tx.creator.end(),
                std::back_inserter(concat_hash_commands_));
      std::vector<uint8_t> concat_hash_commands(concat_hash_commands_.begin(),
                                                concat_hash_commands_.end());

      auto concat_hash =
          sha3_256(concat_hash_commands.data(), concat_hash_commands.size());
      return concat_hash;
    }
  }
}