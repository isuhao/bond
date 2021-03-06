// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "exception.h"
#include "schema.h"
#include "apply.h"
#include "detail/validate.h"

namespace bond
{

/// @brief Validate compatibility of schemas
/// @param src Source schema
/// @param dst Destination schema
/// @return 'true' if schemas are wire-format equivalent, 'false' if schemas
/// are different but payload in source schema can be deserialized as destination.
/// @throw SchemaValidateException if payload in source schema is incompatible
/// with destination schema.
inline bool Validate(const RuntimeSchema& src,
                     const RuntimeSchema& dst)
{
    // Create instances to drop the shared_ptr member of the original objects
    // for performance
    RuntimeSchema r_dst(RuntimeSchema(dst.GetSchema()), dst.GetType());
    RuntimeSchema r_src(RuntimeSchema(src.GetSchema()), src.GetType());
    bool identical = true;
    detail::ValidateStruct(r_src, r_dst, NULL, identical);
    return identical;
}


/// @brief Validate compatibility of schemas
/// @param src Serialized source SchemaDef
/// @param dst Destination schema
/// @return 'true' if schemas are wire-format equivalent, 'false' if schemas
/// are different but payload in source schema can be deserialized as destination.
/// @throw SchemaValidateException if payload in source schema is incompatible
/// with destination schema or the schema of source SchemaDef is unknown.
inline bool Validate(const bonded<SchemaDef>& src,
                     const RuntimeSchema& dst)
{
    Apply(detail::SchemaValidator(), src);

    SchemaDef schema;
    src.Deserialize(schema);
    return Validate(RuntimeSchema(schema), dst);
}


/// @brief Validate compatibility of schemas
/// @param src Source schema
/// @param dst Serialized destination SchemaDef
/// @return 'true' if schemas are wire-format equivalent, 'false' if schemas
/// are different but payload in source schema can be deserialized as destination.
/// @throw SchemaValidateException if payload in source schema is incompatible
/// with destination schema or the schema of destination SchemaDef is unknown.
inline bool Validate(const RuntimeSchema& src,
                     const bonded<SchemaDef>& dst)
{
    Apply(detail::SchemaValidator(), dst);

    SchemaDef schema;
    dst.Deserialize(schema);
    return Validate(src, RuntimeSchema(schema));
}


/// @brief Validate two-way compatibility of schemas
/// @param s1 Schema to compare
/// @param s2 Schema to compare
/// @return 'true' if schemas are wire-format equivalent, 'false' if schemas
/// are different but payload in source can be deserialized as destination, 
/// and vice versa.
/// @throw SchemaValidateException if payload in any one schema is incompatible
/// with the other schema.
template <typename T1, typename T2>
inline bool ValidateTwoWay(const T1& s1, const T2& s2)
{
    return Validate(s1, s2) & Validate(s2, s1);
}

} // namespace bond
