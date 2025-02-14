﻿/* Copyright (c) 2022 AntGroup. All Rights Reserved. */

#include "record.h"

#include "execution_plan/runtime_context.h"
#include "parser/symbol_table.h"

namespace cypher {
lgraph::FieldData Entry::GetEntityField(RTContext *ctx, const std::string &fd) const {
    switch (type) {
    case NODE:
        {
            auto vit = node->ItRef();
            CYPHER_THROW_ASSERT(node && vit);
            return node->IsValidAfterMaterialize(ctx) ? vit->GetField(fd) : lgraph::FieldData();
        }
    case RELATIONSHIP:
        {
            auto eit = relationship->ItRef();
            CYPHER_THROW_ASSERT(relationship && eit);
            return eit->IsValid() ? eit->GetField(fd) : lgraph::FieldData();
        }
    case NODE_SNAPSHOT:
        {
            // extract vid from snapshot, "V[{id}]"
            CYPHER_THROW_ASSERT(constant.type == cypher::FieldData::SCALAR &&
                                constant.scalar.type == lgraph::FieldType::STRING);
            auto vid =
                std::stoi(constant.scalar.string().substr(2, constant.scalar.string().size() - 3));
            return ctx->txn_->GetVertexField(vid, fd);
        }
    case RELP_SNAPSHOT:
    default:
        CYPHER_TODO();
    }
}

void Record::SetParameter(const PARAM_TAB &ptab) {
    if (!symbol_table) return;
    for (auto &param : ptab) {
        auto it = symbol_table->symbols.find(param.first);
        if (it != symbol_table->symbols.end()) {
            values[it->second.id] = Entry(param.second);
        } else {
            // FMA_WARN() << "Invalid parameter: " << param.first;
        }
    }
}
}  // namespace cypher
