/*-------------------------------------------------------------------------
 * Copyright 2025 IvorySQL Global Development Team
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * numeric_datatype_functions.c
 *
 * This file contains the implementation of Oracle's
 * numeric data type related built-in functions.
 *
 * Portions Copyright (c) 2023-2026, IvorySQL Global Development Team
 *
 * contrib/ivorysql_ora/src/builtin_functions/numeric_datatype_functions.c
 *
 * add the file for Oracle's built-in data types
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"
#include <math.h>
#include "fmgr.h"
#include "utils/formatting.h"
#include "utils/numeric.h"

PG_FUNCTION_INFO_V1(number_bitand);
PG_FUNCTION_INFO_V1(ora_to_number);
PG_FUNCTION_INFO_V1(number_nanvl);
PG_FUNCTION_INFO_V1(binary_float_nanvl);
PG_FUNCTION_INFO_V1(binary_double_nanvl);


Datum
number_bitand(PG_FUNCTION_ARGS)
{
	Numeric		arg1 = PG_GETARG_NUMERIC(0);
	Numeric		arg2 = PG_GETARG_NUMERIC(1);

	PG_RETURN_NUMERIC(numeric_bitand(arg1, arg2));
}

/*
 * ora_to_number
 * converts text to a value of NUMBER data type.
 */
Datum
ora_to_number(PG_FUNCTION_ARGS)
{
	text	   *value = PG_GETARG_TEXT_P(0);
	text	   *fmt = NULL;
	Numeric	result;
	int num = PG_NARGS();

	if(num > 1)
		fmt = PG_GETARG_TEXT_P(1);

	result = ora_to_number_internal(value, fmt);

	if(result == NULL)
		PG_RETURN_NULL();
	else
		PG_RETURN_NUMERIC(result);
}

/*
 * number_nanvl
 * Oracle NANVL(expr1, expr2) for NUMBER: returns expr2 when expr1
 * is NaN, otherwise returns expr1. PostgreSQL's numeric type (unlike
 * Oracle's NUMBER) can hold a NaN value, so this reuses the existing
 * numeric_is_nan() rather than assuming expr1 is never NaN.
 */
Datum
number_nanvl(PG_FUNCTION_ARGS)
{
	Numeric		arg1 = PG_GETARG_NUMERIC(0);
	Numeric		arg2 = PG_GETARG_NUMERIC(1);

	PG_RETURN_NUMERIC(numeric_is_nan(arg1) ? arg2 : arg1);
}

/*
 * binary_float_nanvl
 * Oracle NANVL(expr1, expr2) for BINARY_FLOAT: returns expr2 when
 * expr1 is NaN, otherwise returns expr1.
 */
Datum
binary_float_nanvl(PG_FUNCTION_ARGS)
{
	float4		arg1 = PG_GETARG_FLOAT4(0);
	float4		arg2 = PG_GETARG_FLOAT4(1);

	PG_RETURN_FLOAT4(isnan(arg1) ? arg2 : arg1);
}

/*
 * binary_double_nanvl
 * Oracle NANVL(expr1, expr2) for BINARY_DOUBLE: returns expr2 when
 * expr1 is NaN, otherwise returns expr1.
 */
Datum
binary_double_nanvl(PG_FUNCTION_ARGS)
{
	float8		arg1 = PG_GETARG_FLOAT8(0);
	float8		arg2 = PG_GETARG_FLOAT8(1);

	PG_RETURN_FLOAT8(isnan(arg1) ? arg2 : arg1);
}
