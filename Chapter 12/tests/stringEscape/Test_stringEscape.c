#include "../../stringEscape.h"
#include "../../makeMemory.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <string.h>


/*--------------------------------------------------------------*/

void translateEscape_ShouldReturnNullOnInvalidParam(void)
{
    //	fixture setup
    char* nullString = NULL;
    //	system under control
    char* actual = translateEscape(nullString);
    //	verify
    CU_ASSERT_EQUAL(actual, NULL);
}

void translateEscape_ShouldNotChangeOnNoneEscapingString(void)
{
    //	fixture setup
    char* alpha_num_string = "abz0129";
    char* special_string = "[]{~}!@#$%^&*)<>?:|,./;[]'";
    char* whitespace_string = " ";
    //	system under control
    char* alpha_num_actual = translateEscape(alpha_num_string);
    char* special_actual = translateEscape(special_string);
    char* whitespace_actual = translateEscape(whitespace_string);

    //	verify
    CU_ASSERT_STRING_EQUAL(alpha_num_actual, alpha_num_string);
    CU_ASSERT_STRING_EQUAL(special_actual, special_string);
    CU_ASSERT_STRING_EQUAL(whitespace_actual, whitespace_string);
}

/*-------------------------------------*/

void translateEscape_ShouldWorkOnSingleEscapeCharString(void)
{
    //	fixture setup
    char* newline_string = "\\n";
    char* tab_string = "\\t";
    char* double_quote_string = "\\\"";
    char* backslash_string = "\\\\";

    //	system under control
    char* actual_newline = translateEscape(newline_string);
    char* actual_tab = translateEscape(tab_string);
    char* actual_double_quote = translateEscape(double_quote_string);
    char* actual_backslash = translateEscape(backslash_string);

    //	verify
    CU_ASSERT(strlen(actual_newline) == 2 &&
		    strncmp(actual_newline, "\\n", 2) == 0);
    CU_ASSERT(strlen(actual_tab) == 2 &&
		    strncmp(actual_tab, "\\t", 2) == 0);
    CU_ASSERT(strlen(actual_double_quote) == 2 &&
		    strncmp(actual_double_quote, "\\\"", 2) == 0);
    CU_ASSERT(strlen(actual_backslash) == 2 &&
		    strncmp(actual_backslash, "\\\\", 2) == 0);
}

void trsnalteEscape_ShouldNotChangeOnSingleInvalidEscapeString(void)
{
    //	fixture setup
    char* invalid_string = "\\b";
    
    //	system under control
    char* actual = translateEscape(invalid_string);

    //	verify
    CU_ASSERT_STRING_EQUAL(actual, invalid_string);
}

void translateEscape_ShouldWorkRightOnCompositeSingleEscapeString(void)
{
    //	fixture setup
    char* string = "\\\\\\\"\\t\\n";
    char* expected = "\\\\\\\"\\t\\n";

    //	system under control
    char* actual = translateEscape(string);

    //	verify
    CU_ASSERT_STRING_EQUAL(actual, expected);
}

void trsnalteEscape_ShouldWorkRightOnCompositeValidAndInvalidEscapeString(void)
{
    //	fixture setup
    char* invalid_string = "\\b\\t\\r\\n\\f";
    char* expected_string = "\\b\\t\\r\\n\\f";

    //	system under control
    char* actual = translateEscape(invalid_string);

    //	verify
    CU_ASSERT_STRING_EQUAL(actual, expected_string);
}

/*--------------------------------------*/

void translateEscape_ShouldWorkOnControlEscapeString(void)
{
    //	fixture setup
    char* control_str = "\\^@\\^A\\^G\\^Z\\^\\\\^[\\^]\\^^\\^_";
    char* expected = "\x00\x01\x07\x1A\x1C\x1B\x1D\x1E\x1F";

    //	system under control
    char* actual = translateEscape(control_str);

    //	verify
    CU_ASSERT_STRING_EQUAL(actual, expected);
}

void translateEscape_ShouldNotChangeOnInvalidControlEscapeString(void)
{
    //	fixture setup
    char* invalid_string = "\\^!\\^%\\^*\\^a\\^g\\^z";

    //	system under control
    char* actual = translateEscape(invalid_string);

    //	verify
    CU_ASSERT_STRING_EQUAL(actual, invalid_string);
}

void translateEscape_ShouldWorkRightOnCompositeValidAndInvalidEscapeString(void)
{
    //	fixture setup
    char* string = "\\^%\\^A\\^a\\^G\\^b\\^Z\\^z\\^[";
    char* expected = "\\^%\x01\\^a\x07\\^b\x1A\\^z\x1B";

    //	system under control
    char* actual = translateEscape(string);

    //	verify
    CU_ASSERT_STRING_EQUAL(actual, expected);
}

/*------------------------------------------*/

void translateEscape_ShouldWorkOnEscapeNumberString(void)
{
    //	fixture setup
    char* escape_string = "\\000\\056\\177";
    char* expected = "\000\056\177";
    
    //	system under control
    char* actual = translateEscape(escape_string);

    //	verify
    CU_ASSERT_STRING_EQUAL(actual, expected);
}

void translateEscape_OnlyTranslateThreeOctalEscapeString(void)
{
    //	fixture setup
    char* string = "\\1\\12\\178\\000\\200\\800\\0000";
    char* expected = "\\1\\12\\178\000\200\\800\\0000";

    //	system under control
    char* actual = translateEscape(string);

    //	verify
    CU_ASSERT_STRING_EQUAL(actual, expected);
}

void translateEscape_ShouldRemoveValidMultilineSequence(void)
{
    //	fixture setup
    char* str_newline = "\\\n\\";
    char* str_space_before_newline = "\\ \n\\";
    char* str_space_after_newline = "\\\n \\";
    char* str_spaces_newline = "\\ \n \\";
    char* str_tab_before_newline = "\\\t\n\\";
    char* str_tab_after_newline = "\\\n\t\\";
    char* str_tabs_newline = "\\\t\n\t\\";
    char* str_multi_newline = "\\ \n \\\\ \n \\";
    char* str_multiline_sequence = "str\\ \n \\ string \\ \n \\str";

    char* expected = "";
    char* expected_multiline_sequence = "str string str";

    //	system under control
    char* actual_newline = translateEscape(str_newline);
    char* actual_space_before_newline =
	translateEscape(str_space_before_newline);
    char* actual_space_after_newline =
	translateEscape(str_space_after_newline);
    char* actual_spaces_newline = translateEscape(str_spaces_newline);
    char* actual_tab_before_newline =
	translateEscape(str_tab_before_newline);
    char* actual_tab_after_newline =
	translateEscape(str_tab_after_newline);
    char* actual_tabs_newline = translateEscape(str_tabs_newline);
    char* actual_multi_newline = translateEscape(str_multi_newline);
    char* actual_multiline_sequence =
	translateEscape(str_multiline_sequence);
    
    //	verify
    CU_ASSERT_STRING_EQUAL(actual_newline, expected);
    CU_ASSERT_STRING_EQUAL(actual_space_before_newline, expected);
    CU_ASSERT_STRING_EQUAL(actual_space_after_newline, expected);
    CU_ASSERT_STRING_EQUAL(actual_spaces_newline, expected);
    CU_ASSERT_STRING_EQUAL(actual_tab_before_newline, expected);
    CU_ASSERT_STRING_EQUAL(actual_tab_after_newline, expected);
    CU_ASSERT_STRING_EQUAL(actual_tabs_newline, expected);
    CU_ASSERT_STRING_EQUAL(actual_multi_newline, expected);
    CU_ASSERT_STRING_EQUAL(actual_multiline_sequence, expected_multiline_sequence);
}

/*--------------------------------------*/

void translateEscape_ShouldWorkRightWithAllEscapeTogether(void)
{
    //	fixture setup
    char* str = "\\t1:\\\\\001\\ \n \\\\\"\\n\\^A";
    char* expected = "\\t1:\\\\\x01\\\"\\n\x01";
    
    //	system under control
    char* actual = translateEscape(str);

    //	verify
    CU_ASSERT_STRING_EQUAL(actual, expected);
}

/*--------------------------------------------------------------*/

int main (int argc, char* argv[])
{
    if (CU_initialize_registry() != CUE_SUCCESS)
    {
	printf("Failed to initialize registry!\n");
	exit(-1);
    }

    CU_pSuite suite = CU_add_suite("suite for makeMemory", NULL, NULL);
    if (suite == NULL)
    {
	printf("Failed to add suite!\n");
	exit(-1);
    }

    //	add tests
    CU_TestInfo tests[] = {
	{ "test func----translateEscape()", translateEscape_ShouldReturnNullOnInvalidParam },
	{ "test func----translateEscape()", translateEscape_ShouldNotChangeOnNoneEscapingString },
	{ "test func----translateEscape()", translateEscape_ShouldWorkOnSingleEscapeCharString },
	{ "test func----translateEscape()", translateEscape_ShouldWorkRightOnCompositeSingleEscapeString },
	{ "test func----translateEscape()", translateEscape_ShouldWorkOnControlEscapeString },
	{ "test func----translateEscape()" , translateEscape_ShouldWorkOnEscapeNumberString },
	{ "test func----translateEscape()", trsnalteEscape_ShouldNotChangeOnSingleInvalidEscapeString },
	{ "test func----translateEscape()", trsnalteEscape_ShouldWorkRightOnCompositeValidAndInvalidEscapeString },
	{ "test func----translateEscape()", translateEscape_ShouldNotChangeOnInvalidControlEscapeString },
	{ "test func----translateEscape()", translateEscape_ShouldWorkRightOnCompositeValidAndInvalidEscapeString },
	{ "test func----translateEscape()", translateEscape_OnlyTranslateThreeOctalEscapeString },
	{ "test func----translateEscape()", translateEscape_ShouldRemoveValidMultilineSequence },
	{ "test func----translateEscape()", translateEscape_ShouldWorkRightWithAllEscapeTogether }
    };
    
    for (unsigned i = 0; i < (sizeof(tests) / sizeof(tests[0])); ++i)
	if (CU_add_test(suite, tests[i].pName, tests[i].pTestFunc) == NULL)
	{
	    printf("attempt to add tests, failed!\n");
	    exit(-1);
	}

    if (CU_basic_run_suite(suite) != CUE_SUCCESS)
    {
	printf("Failed to run suite!\n");
	exit(-1);
    }

    CU_cleanup_registry();
    freeMemoryChain();
    return 0;
}
