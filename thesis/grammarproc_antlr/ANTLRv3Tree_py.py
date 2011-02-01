# $ANTLR 3.1.2 /home/res/ANTLRv3Tree_py.g 2009-04-06 19:42:12

import sys
from antlr3 import *
from antlr3.tree import *
from antlr3.compat import set, frozenset


# for convenience in actions
HIDDEN = BaseRecognizer.HIDDEN

# token types
BACKTRACK_SEMPRED=35
DOUBLE_ANGLE_STRING_LITERAL=51
LEXER_GRAMMAR=24
EOA=19
ARGLIST=22
EOF=-1
SEMPRED=32
ACTION=45
EOB=18
TOKEN_REF=42
T__93=93
T__91=91
RET=23
T__92=92
STRING_LITERAL=43
T__90=90
ARG=21
EOR=17
ARG_ACTION=48
DOUBLE_QUOTE_STRING_LITERAL=50
NESTED_ARG_ACTION=58
ACTION_CHAR_LITERAL=60
INITACTION=28
T__80=80
T__81=81
T__82=82
RULE=7
ACTION_ESC=62
T__83=83
PARSER_GRAMMAR=25
SRC=52
INT=47
CHAR_RANGE=14
EPSILON=15
T__85=85
T__84=84
T__87=87
T__86=86
REWRITE=40
T__89=89
T__88=88
WS=64
T__71=71
T__72=72
COMBINED_GRAMMAR=27
T__70=70
LEXER=6
SL_COMMENT=53
TREE_GRAMMAR=26
T__76=76
CLOSURE=10
T__75=75
PARSER=5
T__74=74
T__73=73
T__79=79
T__78=78
T__77=77
T__68=68
T__69=69
T__66=66
T__67=67
T__65=65
NESTED_ACTION=61
ESC=56
FRAGMENT=36
ID=20
TREE_BEGIN=37
ML_COMMENT=54
ALT=16
SCOPE=31
DOC_COMMENT=4
WS_LOOP=63
RANGE=13
TOKENS=41
GATED_SEMPRED=33
LITERAL_CHAR=55
BANG=39
ACTION_STRING_LITERAL=59
ROOT=38
RULE_REF=49
SYNPRED=12
OPTIONAL=9
CHAR_LITERAL=44
LABEL=29
TEMPLATE=30
SYN_SEMPRED=34
XDIGIT=57
BLOCK=8
POSITIVE_CLOSURE=11
OPTIONS=46

# token names
tokenNames = [
    "<invalid>", "<EOR>", "<DOWN>", "<UP>", 
    "DOC_COMMENT", "PARSER", "LEXER", "RULE", "BLOCK", "OPTIONAL", "CLOSURE", 
    "POSITIVE_CLOSURE", "SYNPRED", "RANGE", "CHAR_RANGE", "EPSILON", "ALT", 
    "EOR", "EOB", "EOA", "ID", "ARG", "ARGLIST", "RET", "LEXER_GRAMMAR", 
    "PARSER_GRAMMAR", "TREE_GRAMMAR", "COMBINED_GRAMMAR", "INITACTION", 
    "LABEL", "TEMPLATE", "SCOPE", "SEMPRED", "GATED_SEMPRED", "SYN_SEMPRED", 
    "BACKTRACK_SEMPRED", "FRAGMENT", "TREE_BEGIN", "ROOT", "BANG", "REWRITE", 
    "TOKENS", "TOKEN_REF", "STRING_LITERAL", "CHAR_LITERAL", "ACTION", "OPTIONS", 
    "INT", "ARG_ACTION", "RULE_REF", "DOUBLE_QUOTE_STRING_LITERAL", "DOUBLE_ANGLE_STRING_LITERAL", 
    "SRC", "SL_COMMENT", "ML_COMMENT", "LITERAL_CHAR", "ESC", "XDIGIT", 
    "NESTED_ARG_ACTION", "ACTION_STRING_LITERAL", "ACTION_CHAR_LITERAL", 
    "NESTED_ACTION", "ACTION_ESC", "WS_LOOP", "WS", "'lexer'", "'parser'", 
    "'tree'", "'grammar'", "';'", "'}'", "'='", "'@'", "'::'", "'*'", "'protected'", 
    "'public'", "'private'", "'returns'", "':'", "'throws'", "','", "'('", 
    "'|'", "')'", "'catch'", "'finally'", "'+='", "'=>'", "'~'", "'?'", 
    "'+'", "'.'", "'$'"
]




class ANTLRv3Tree_py(TreeParser):
    grammarFileName = "/home/res/ANTLRv3Tree_py.g"
    antlr_version = version_str_to_tuple("3.1.2")
    antlr_version_str = "3.1.2"
    tokenNames = tokenNames

    def __init__(self, input, state=None):
        if state is None:
            state = RecognizerSharedState()

        TreeParser.__init__(self, input, state)


        self.dfa30 = self.DFA30(
            self, 30,
            eot = self.DFA30_eot,
            eof = self.DFA30_eof,
            min = self.DFA30_min,
            max = self.DFA30_max,
            accept = self.DFA30_accept,
            special = self.DFA30_special,
            transition = self.DFA30_transition
            )

        self.dfa43 = self.DFA43(
            self, 43,
            eot = self.DFA43_eot,
            eof = self.DFA43_eof,
            min = self.DFA43_min,
            max = self.DFA43_max,
            accept = self.DFA43_accept,
            special = self.DFA43_special,
            transition = self.DFA43_transition
            )






                


        



    # $ANTLR start "grammarDef"
    # /home/res/ANTLRv3Tree_py.g:38:1: grammarDef : ^( grammarType ID ( DOC_COMMENT )? ( optionsSpec )? ( tokensSpec )? ( attrScope )* ( action )* ( rule )+ ) ;
    def grammarDef(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:39:5: ( ^( grammarType ID ( DOC_COMMENT )? ( optionsSpec )? ( tokensSpec )? ( attrScope )* ( action )* ( rule )+ ) )
                # /home/res/ANTLRv3Tree_py.g:39:9: ^( grammarType ID ( DOC_COMMENT )? ( optionsSpec )? ( tokensSpec )? ( attrScope )* ( action )* ( rule )+ )
                pass 
                self._state.following.append(self.FOLLOW_grammarType_in_grammarDef52)
                self.grammarType()

                self._state.following.pop()

                self.match(self.input, DOWN, None)
                self.match(self.input, ID, self.FOLLOW_ID_in_grammarDef54)
                # /home/res/ANTLRv3Tree_py.g:39:27: ( DOC_COMMENT )?
                alt1 = 2
                LA1_0 = self.input.LA(1)

                if (LA1_0 == DOC_COMMENT) :
                    alt1 = 1
                if alt1 == 1:
                    # /home/res/ANTLRv3Tree_py.g:39:27: DOC_COMMENT
                    pass 
                    self.match(self.input, DOC_COMMENT, self.FOLLOW_DOC_COMMENT_in_grammarDef56)



                # /home/res/ANTLRv3Tree_py.g:39:40: ( optionsSpec )?
                alt2 = 2
                LA2_0 = self.input.LA(1)

                if (LA2_0 == OPTIONS) :
                    alt2 = 1
                if alt2 == 1:
                    # /home/res/ANTLRv3Tree_py.g:39:40: optionsSpec
                    pass 
                    self._state.following.append(self.FOLLOW_optionsSpec_in_grammarDef59)
                    self.optionsSpec()

                    self._state.following.pop()



                # /home/res/ANTLRv3Tree_py.g:39:53: ( tokensSpec )?
                alt3 = 2
                LA3_0 = self.input.LA(1)

                if (LA3_0 == TOKENS) :
                    alt3 = 1
                if alt3 == 1:
                    # /home/res/ANTLRv3Tree_py.g:39:53: tokensSpec
                    pass 
                    self._state.following.append(self.FOLLOW_tokensSpec_in_grammarDef62)
                    self.tokensSpec()

                    self._state.following.pop()



                # /home/res/ANTLRv3Tree_py.g:39:65: ( attrScope )*
                while True: #loop4
                    alt4 = 2
                    LA4_0 = self.input.LA(1)

                    if (LA4_0 == SCOPE) :
                        alt4 = 1


                    if alt4 == 1:
                        # /home/res/ANTLRv3Tree_py.g:39:65: attrScope
                        pass 
                        self._state.following.append(self.FOLLOW_attrScope_in_grammarDef65)
                        self.attrScope()

                        self._state.following.pop()


                    else:
                        break #loop4


                # /home/res/ANTLRv3Tree_py.g:39:76: ( action )*
                while True: #loop5
                    alt5 = 2
                    LA5_0 = self.input.LA(1)

                    if (LA5_0 == 72) :
                        alt5 = 1


                    if alt5 == 1:
                        # /home/res/ANTLRv3Tree_py.g:39:76: action
                        pass 
                        self._state.following.append(self.FOLLOW_action_in_grammarDef68)
                        self.action()

                        self._state.following.pop()


                    else:
                        break #loop5


                # /home/res/ANTLRv3Tree_py.g:39:84: ( rule )+
                cnt6 = 0
                while True: #loop6
                    alt6 = 2
                    LA6_0 = self.input.LA(1)

                    if (LA6_0 == RULE) :
                        alt6 = 1


                    if alt6 == 1:
                        # /home/res/ANTLRv3Tree_py.g:39:84: rule
                        pass 
                        self._state.following.append(self.FOLLOW_rule_in_grammarDef71)
                        self.rule()

                        self._state.following.pop()


                    else:
                        if cnt6 >= 1:
                            break #loop6

                        eee = EarlyExitException(6, self.input)
                        raise eee

                    cnt6 += 1



                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "grammarDef"


    # $ANTLR start "grammarType"
    # /home/res/ANTLRv3Tree_py.g:42:1: grammarType : ( LEXER_GRAMMAR | PARSER_GRAMMAR | TREE_GRAMMAR | COMBINED_GRAMMAR );
    def grammarType(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:43:2: ( LEXER_GRAMMAR | PARSER_GRAMMAR | TREE_GRAMMAR | COMBINED_GRAMMAR )
                # /home/res/ANTLRv3Tree_py.g:
                pass 
                if (LEXER_GRAMMAR <= self.input.LA(1) <= COMBINED_GRAMMAR):
                    self.input.consume()
                    self._state.errorRecovery = False

                else:
                    mse = MismatchedSetException(None, self.input)
                    raise mse






            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "grammarType"


    # $ANTLR start "tokensSpec"
    # /home/res/ANTLRv3Tree_py.g:49:1: tokensSpec : ^( TOKENS ( tokenSpec )+ ) ;
    def tokensSpec(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:50:2: ( ^( TOKENS ( tokenSpec )+ ) )
                # /home/res/ANTLRv3Tree_py.g:50:4: ^( TOKENS ( tokenSpec )+ )
                pass 
                self.match(self.input, TOKENS, self.FOLLOW_TOKENS_in_tokensSpec127)

                self.match(self.input, DOWN, None)
                # /home/res/ANTLRv3Tree_py.g:50:13: ( tokenSpec )+
                cnt7 = 0
                while True: #loop7
                    alt7 = 2
                    LA7_0 = self.input.LA(1)

                    if (LA7_0 == TOKEN_REF or LA7_0 == 71) :
                        alt7 = 1


                    if alt7 == 1:
                        # /home/res/ANTLRv3Tree_py.g:50:13: tokenSpec
                        pass 
                        self._state.following.append(self.FOLLOW_tokenSpec_in_tokensSpec129)
                        self.tokenSpec()

                        self._state.following.pop()


                    else:
                        if cnt7 >= 1:
                            break #loop7

                        eee = EarlyExitException(7, self.input)
                        raise eee

                    cnt7 += 1



                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "tokensSpec"


    # $ANTLR start "tokenSpec"
    # /home/res/ANTLRv3Tree_py.g:53:1: tokenSpec : ( ^( '=' TOKEN_REF STRING_LITERAL ) | ^( '=' TOKEN_REF CHAR_LITERAL ) | TOKEN_REF );
    def tokenSpec(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:54:2: ( ^( '=' TOKEN_REF STRING_LITERAL ) | ^( '=' TOKEN_REF CHAR_LITERAL ) | TOKEN_REF )
                alt8 = 3
                LA8_0 = self.input.LA(1)

                if (LA8_0 == 71) :
                    LA8_1 = self.input.LA(2)

                    if (LA8_1 == 2) :
                        LA8_3 = self.input.LA(3)

                        if (LA8_3 == TOKEN_REF) :
                            LA8_4 = self.input.LA(4)

                            if (LA8_4 == STRING_LITERAL) :
                                alt8 = 1
                            elif (LA8_4 == CHAR_LITERAL) :
                                alt8 = 2
                            else:
                                nvae = NoViableAltException("", 8, 4, self.input)

                                raise nvae

                        else:
                            nvae = NoViableAltException("", 8, 3, self.input)

                            raise nvae

                    else:
                        nvae = NoViableAltException("", 8, 1, self.input)

                        raise nvae

                elif (LA8_0 == TOKEN_REF) :
                    alt8 = 3
                else:
                    nvae = NoViableAltException("", 8, 0, self.input)

                    raise nvae

                if alt8 == 1:
                    # /home/res/ANTLRv3Tree_py.g:54:4: ^( '=' TOKEN_REF STRING_LITERAL )
                    pass 
                    self.match(self.input, 71, self.FOLLOW_71_in_tokenSpec143)

                    self.match(self.input, DOWN, None)
                    self.match(self.input, TOKEN_REF, self.FOLLOW_TOKEN_REF_in_tokenSpec145)
                    self.match(self.input, STRING_LITERAL, self.FOLLOW_STRING_LITERAL_in_tokenSpec147)

                    self.match(self.input, UP, None)


                elif alt8 == 2:
                    # /home/res/ANTLRv3Tree_py.g:55:4: ^( '=' TOKEN_REF CHAR_LITERAL )
                    pass 
                    self.match(self.input, 71, self.FOLLOW_71_in_tokenSpec154)

                    self.match(self.input, DOWN, None)
                    self.match(self.input, TOKEN_REF, self.FOLLOW_TOKEN_REF_in_tokenSpec156)
                    self.match(self.input, CHAR_LITERAL, self.FOLLOW_CHAR_LITERAL_in_tokenSpec158)

                    self.match(self.input, UP, None)


                elif alt8 == 3:
                    # /home/res/ANTLRv3Tree_py.g:56:4: TOKEN_REF
                    pass 
                    self.match(self.input, TOKEN_REF, self.FOLLOW_TOKEN_REF_in_tokenSpec164)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "tokenSpec"


    # $ANTLR start "attrScope"
    # /home/res/ANTLRv3Tree_py.g:59:1: attrScope : ^( 'scope' ID ACTION ) ;
    def attrScope(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:60:2: ( ^( 'scope' ID ACTION ) )
                # /home/res/ANTLRv3Tree_py.g:60:4: ^( 'scope' ID ACTION )
                pass 
                self.match(self.input, SCOPE, self.FOLLOW_SCOPE_in_attrScope176)

                self.match(self.input, DOWN, None)
                self.match(self.input, ID, self.FOLLOW_ID_in_attrScope178)
                self.match(self.input, ACTION, self.FOLLOW_ACTION_in_attrScope180)

                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "attrScope"


    # $ANTLR start "action"
    # /home/res/ANTLRv3Tree_py.g:63:1: action : ( ^( '@' ID ID ACTION ) | ^( '@' ID ACTION ) );
    def action(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:64:2: ( ^( '@' ID ID ACTION ) | ^( '@' ID ACTION ) )
                alt9 = 2
                LA9_0 = self.input.LA(1)

                if (LA9_0 == 72) :
                    LA9_1 = self.input.LA(2)

                    if (LA9_1 == 2) :
                        LA9_2 = self.input.LA(3)

                        if (LA9_2 == ID) :
                            LA9_3 = self.input.LA(4)

                            if (LA9_3 == ID) :
                                alt9 = 1
                            elif (LA9_3 == ACTION) :
                                alt9 = 2
                            else:
                                nvae = NoViableAltException("", 9, 3, self.input)

                                raise nvae

                        else:
                            nvae = NoViableAltException("", 9, 2, self.input)

                            raise nvae

                    else:
                        nvae = NoViableAltException("", 9, 1, self.input)

                        raise nvae

                else:
                    nvae = NoViableAltException("", 9, 0, self.input)

                    raise nvae

                if alt9 == 1:
                    # /home/res/ANTLRv3Tree_py.g:64:4: ^( '@' ID ID ACTION )
                    pass 
                    self.match(self.input, 72, self.FOLLOW_72_in_action193)

                    self.match(self.input, DOWN, None)
                    self.match(self.input, ID, self.FOLLOW_ID_in_action195)
                    self.match(self.input, ID, self.FOLLOW_ID_in_action197)
                    self.match(self.input, ACTION, self.FOLLOW_ACTION_in_action199)

                    self.match(self.input, UP, None)


                elif alt9 == 2:
                    # /home/res/ANTLRv3Tree_py.g:65:4: ^( '@' ID ACTION )
                    pass 
                    self.match(self.input, 72, self.FOLLOW_72_in_action206)

                    self.match(self.input, DOWN, None)
                    self.match(self.input, ID, self.FOLLOW_ID_in_action208)
                    self.match(self.input, ACTION, self.FOLLOW_ACTION_in_action210)

                    self.match(self.input, UP, None)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "action"


    # $ANTLR start "optionsSpec"
    # /home/res/ANTLRv3Tree_py.g:68:1: optionsSpec : ^( OPTIONS ( option )+ ) ;
    def optionsSpec(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:69:2: ( ^( OPTIONS ( option )+ ) )
                # /home/res/ANTLRv3Tree_py.g:69:4: ^( OPTIONS ( option )+ )
                pass 
                self.match(self.input, OPTIONS, self.FOLLOW_OPTIONS_in_optionsSpec223)

                self.match(self.input, DOWN, None)
                # /home/res/ANTLRv3Tree_py.g:69:14: ( option )+
                cnt10 = 0
                while True: #loop10
                    alt10 = 2
                    LA10_0 = self.input.LA(1)

                    if (LA10_0 == 71) :
                        alt10 = 1


                    if alt10 == 1:
                        # /home/res/ANTLRv3Tree_py.g:69:14: option
                        pass 
                        self._state.following.append(self.FOLLOW_option_in_optionsSpec225)
                        self.option()

                        self._state.following.pop()


                    else:
                        if cnt10 >= 1:
                            break #loop10

                        eee = EarlyExitException(10, self.input)
                        raise eee

                    cnt10 += 1



                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "optionsSpec"


    # $ANTLR start "option"
    # /home/res/ANTLRv3Tree_py.g:72:1: option : ^( '=' ID optionValue ) ;
    def option(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:73:5: ( ^( '=' ID optionValue ) )
                # /home/res/ANTLRv3Tree_py.g:73:9: ^( '=' ID optionValue )
                pass 
                self.match(self.input, 71, self.FOLLOW_71_in_option244)

                self.match(self.input, DOWN, None)
                self.match(self.input, ID, self.FOLLOW_ID_in_option246)
                self._state.following.append(self.FOLLOW_optionValue_in_option248)
                self.optionValue()

                self._state.following.pop()

                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "option"


    # $ANTLR start "optionValue"
    # /home/res/ANTLRv3Tree_py.g:76:1: optionValue : ( ID | STRING_LITERAL | CHAR_LITERAL | INT );
    def optionValue(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:77:5: ( ID | STRING_LITERAL | CHAR_LITERAL | INT )
                # /home/res/ANTLRv3Tree_py.g:
                pass 
                if self.input.LA(1) == ID or (STRING_LITERAL <= self.input.LA(1) <= CHAR_LITERAL) or self.input.LA(1) == INT:
                    self.input.consume()
                    self._state.errorRecovery = False

                else:
                    mse = MismatchedSetException(None, self.input)
                    raise mse






            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "optionValue"


    # $ANTLR start "rule"
    # /home/res/ANTLRv3Tree_py.g:83:1: rule : ^( RULE ID ( modifier )? ( ^( ARG ARG_ACTION ) )? ( ^( RET ARG_ACTION ) )? ( optionsSpec )? ( ruleScopeSpec )? ( ruleAction )* altList ( exceptionGroup )? EOR ) ;
    def rule(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:84:2: ( ^( RULE ID ( modifier )? ( ^( ARG ARG_ACTION ) )? ( ^( RET ARG_ACTION ) )? ( optionsSpec )? ( ruleScopeSpec )? ( ruleAction )* altList ( exceptionGroup )? EOR ) )
                # /home/res/ANTLRv3Tree_py.g:84:4: ^( RULE ID ( modifier )? ( ^( ARG ARG_ACTION ) )? ( ^( RET ARG_ACTION ) )? ( optionsSpec )? ( ruleScopeSpec )? ( ruleAction )* altList ( exceptionGroup )? EOR )
                pass 
                self.match(self.input, RULE, self.FOLLOW_RULE_in_rule314)

                self.match(self.input, DOWN, None)
                self.match(self.input, ID, self.FOLLOW_ID_in_rule316)
                # /home/res/ANTLRv3Tree_py.g:84:15: ( modifier )?
                alt11 = 2
                LA11_0 = self.input.LA(1)

                if (LA11_0 == FRAGMENT or (75 <= LA11_0 <= 77)) :
                    alt11 = 1
                if alt11 == 1:
                    # /home/res/ANTLRv3Tree_py.g:84:15: modifier
                    pass 
                    self._state.following.append(self.FOLLOW_modifier_in_rule318)
                    self.modifier()

                    self._state.following.pop()



                # /home/res/ANTLRv3Tree_py.g:84:25: ( ^( ARG ARG_ACTION ) )?
                alt12 = 2
                LA12_0 = self.input.LA(1)

                if (LA12_0 == ARG) :
                    alt12 = 1
                if alt12 == 1:
                    # /home/res/ANTLRv3Tree_py.g:84:26: ^( ARG ARG_ACTION )
                    pass 
                    self.match(self.input, ARG, self.FOLLOW_ARG_in_rule323)

                    self.match(self.input, DOWN, None)
                    self.match(self.input, ARG_ACTION, self.FOLLOW_ARG_ACTION_in_rule325)

                    self.match(self.input, UP, None)



                # /home/res/ANTLRv3Tree_py.g:84:46: ( ^( RET ARG_ACTION ) )?
                alt13 = 2
                LA13_0 = self.input.LA(1)

                if (LA13_0 == RET) :
                    alt13 = 1
                if alt13 == 1:
                    # /home/res/ANTLRv3Tree_py.g:84:47: ^( RET ARG_ACTION )
                    pass 
                    self.match(self.input, RET, self.FOLLOW_RET_in_rule332)

                    self.match(self.input, DOWN, None)
                    self.match(self.input, ARG_ACTION, self.FOLLOW_ARG_ACTION_in_rule334)

                    self.match(self.input, UP, None)



                # /home/res/ANTLRv3Tree_py.g:85:9: ( optionsSpec )?
                alt14 = 2
                LA14_0 = self.input.LA(1)

                if (LA14_0 == OPTIONS) :
                    alt14 = 1
                if alt14 == 1:
                    # /home/res/ANTLRv3Tree_py.g:85:9: optionsSpec
                    pass 
                    self._state.following.append(self.FOLLOW_optionsSpec_in_rule347)
                    self.optionsSpec()

                    self._state.following.pop()



                # /home/res/ANTLRv3Tree_py.g:85:22: ( ruleScopeSpec )?
                alt15 = 2
                LA15_0 = self.input.LA(1)

                if (LA15_0 == SCOPE) :
                    alt15 = 1
                if alt15 == 1:
                    # /home/res/ANTLRv3Tree_py.g:85:22: ruleScopeSpec
                    pass 
                    self._state.following.append(self.FOLLOW_ruleScopeSpec_in_rule350)
                    self.ruleScopeSpec()

                    self._state.following.pop()



                # /home/res/ANTLRv3Tree_py.g:85:37: ( ruleAction )*
                while True: #loop16
                    alt16 = 2
                    LA16_0 = self.input.LA(1)

                    if (LA16_0 == 72) :
                        alt16 = 1


                    if alt16 == 1:
                        # /home/res/ANTLRv3Tree_py.g:85:37: ruleAction
                        pass 
                        self._state.following.append(self.FOLLOW_ruleAction_in_rule353)
                        self.ruleAction()

                        self._state.following.pop()


                    else:
                        break #loop16


                self._state.following.append(self.FOLLOW_altList_in_rule364)
                self.altList()

                self._state.following.pop()
                # /home/res/ANTLRv3Tree_py.g:87:9: ( exceptionGroup )?
                alt17 = 2
                LA17_0 = self.input.LA(1)

                if ((85 <= LA17_0 <= 86)) :
                    alt17 = 1
                if alt17 == 1:
                    # /home/res/ANTLRv3Tree_py.g:87:9: exceptionGroup
                    pass 
                    self._state.following.append(self.FOLLOW_exceptionGroup_in_rule374)
                    self.exceptionGroup()

                    self._state.following.pop()



                self.match(self.input, EOR, self.FOLLOW_EOR_in_rule377)

                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "rule"


    # $ANTLR start "modifier"
    # /home/res/ANTLRv3Tree_py.g:91:1: modifier : ( 'protected' | 'public' | 'private' | 'fragment' );
    def modifier(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:92:2: ( 'protected' | 'public' | 'private' | 'fragment' )
                # /home/res/ANTLRv3Tree_py.g:
                pass 
                if self.input.LA(1) == FRAGMENT or (75 <= self.input.LA(1) <= 77):
                    self.input.consume()
                    self._state.errorRecovery = False

                else:
                    mse = MismatchedSetException(None, self.input)
                    raise mse






            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "modifier"


    # $ANTLR start "ruleAction"
    # /home/res/ANTLRv3Tree_py.g:95:1: ruleAction : ^( '@' ID ACTION ) ;
    def ruleAction(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:97:2: ( ^( '@' ID ACTION ) )
                # /home/res/ANTLRv3Tree_py.g:97:4: ^( '@' ID ACTION )
                pass 
                self.match(self.input, 72, self.FOLLOW_72_in_ruleAction416)

                self.match(self.input, DOWN, None)
                self.match(self.input, ID, self.FOLLOW_ID_in_ruleAction418)
                self.match(self.input, ACTION, self.FOLLOW_ACTION_in_ruleAction420)

                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "ruleAction"


    # $ANTLR start "throwsSpec"
    # /home/res/ANTLRv3Tree_py.g:100:1: throwsSpec : ^( 'throws' ( ID )+ ) ;
    def throwsSpec(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:101:2: ( ^( 'throws' ( ID )+ ) )
                # /home/res/ANTLRv3Tree_py.g:101:4: ^( 'throws' ( ID )+ )
                pass 
                self.match(self.input, 80, self.FOLLOW_80_in_throwsSpec433)

                self.match(self.input, DOWN, None)
                # /home/res/ANTLRv3Tree_py.g:101:15: ( ID )+
                cnt18 = 0
                while True: #loop18
                    alt18 = 2
                    LA18_0 = self.input.LA(1)

                    if (LA18_0 == ID) :
                        alt18 = 1


                    if alt18 == 1:
                        # /home/res/ANTLRv3Tree_py.g:101:15: ID
                        pass 
                        self.match(self.input, ID, self.FOLLOW_ID_in_throwsSpec435)


                    else:
                        if cnt18 >= 1:
                            break #loop18

                        eee = EarlyExitException(18, self.input)
                        raise eee

                    cnt18 += 1



                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "throwsSpec"


    # $ANTLR start "ruleScopeSpec"
    # /home/res/ANTLRv3Tree_py.g:104:1: ruleScopeSpec : ( ^( 'scope' ACTION ) | ^( 'scope' ACTION ( ID )+ ) | ^( 'scope' ( ID )+ ) );
    def ruleScopeSpec(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:105:2: ( ^( 'scope' ACTION ) | ^( 'scope' ACTION ( ID )+ ) | ^( 'scope' ( ID )+ ) )
                alt21 = 3
                LA21_0 = self.input.LA(1)

                if (LA21_0 == SCOPE) :
                    LA21_1 = self.input.LA(2)

                    if (LA21_1 == 2) :
                        LA21_2 = self.input.LA(3)

                        if (LA21_2 == ACTION) :
                            LA21_3 = self.input.LA(4)

                            if (LA21_3 == 3) :
                                alt21 = 1
                            elif (LA21_3 == ID) :
                                alt21 = 2
                            else:
                                nvae = NoViableAltException("", 21, 3, self.input)

                                raise nvae

                        elif (LA21_2 == ID) :
                            alt21 = 3
                        else:
                            nvae = NoViableAltException("", 21, 2, self.input)

                            raise nvae

                    else:
                        nvae = NoViableAltException("", 21, 1, self.input)

                        raise nvae

                else:
                    nvae = NoViableAltException("", 21, 0, self.input)

                    raise nvae

                if alt21 == 1:
                    # /home/res/ANTLRv3Tree_py.g:105:4: ^( 'scope' ACTION )
                    pass 
                    self.match(self.input, SCOPE, self.FOLLOW_SCOPE_in_ruleScopeSpec449)

                    self.match(self.input, DOWN, None)
                    self.match(self.input, ACTION, self.FOLLOW_ACTION_in_ruleScopeSpec451)

                    self.match(self.input, UP, None)


                elif alt21 == 2:
                    # /home/res/ANTLRv3Tree_py.g:106:4: ^( 'scope' ACTION ( ID )+ )
                    pass 
                    self.match(self.input, SCOPE, self.FOLLOW_SCOPE_in_ruleScopeSpec458)

                    self.match(self.input, DOWN, None)
                    self.match(self.input, ACTION, self.FOLLOW_ACTION_in_ruleScopeSpec460)
                    # /home/res/ANTLRv3Tree_py.g:106:21: ( ID )+
                    cnt19 = 0
                    while True: #loop19
                        alt19 = 2
                        LA19_0 = self.input.LA(1)

                        if (LA19_0 == ID) :
                            alt19 = 1


                        if alt19 == 1:
                            # /home/res/ANTLRv3Tree_py.g:106:21: ID
                            pass 
                            self.match(self.input, ID, self.FOLLOW_ID_in_ruleScopeSpec462)


                        else:
                            if cnt19 >= 1:
                                break #loop19

                            eee = EarlyExitException(19, self.input)
                            raise eee

                        cnt19 += 1



                    self.match(self.input, UP, None)


                elif alt21 == 3:
                    # /home/res/ANTLRv3Tree_py.g:107:4: ^( 'scope' ( ID )+ )
                    pass 
                    self.match(self.input, SCOPE, self.FOLLOW_SCOPE_in_ruleScopeSpec470)

                    self.match(self.input, DOWN, None)
                    # /home/res/ANTLRv3Tree_py.g:107:14: ( ID )+
                    cnt20 = 0
                    while True: #loop20
                        alt20 = 2
                        LA20_0 = self.input.LA(1)

                        if (LA20_0 == ID) :
                            alt20 = 1


                        if alt20 == 1:
                            # /home/res/ANTLRv3Tree_py.g:107:14: ID
                            pass 
                            self.match(self.input, ID, self.FOLLOW_ID_in_ruleScopeSpec472)


                        else:
                            if cnt20 >= 1:
                                break #loop20

                            eee = EarlyExitException(20, self.input)
                            raise eee

                        cnt20 += 1



                    self.match(self.input, UP, None)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "ruleScopeSpec"


    # $ANTLR start "block"
    # /home/res/ANTLRv3Tree_py.g:110:1: block : ^( BLOCK ( optionsSpec )? ( alternative rewrite )+ EOB ) ;
    def block(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:111:5: ( ^( BLOCK ( optionsSpec )? ( alternative rewrite )+ EOB ) )
                # /home/res/ANTLRv3Tree_py.g:111:9: ^( BLOCK ( optionsSpec )? ( alternative rewrite )+ EOB )
                pass 
                self.match(self.input, BLOCK, self.FOLLOW_BLOCK_in_block492)

                self.match(self.input, DOWN, None)
                # /home/res/ANTLRv3Tree_py.g:111:18: ( optionsSpec )?
                alt22 = 2
                LA22_0 = self.input.LA(1)

                if (LA22_0 == OPTIONS) :
                    alt22 = 1
                if alt22 == 1:
                    # /home/res/ANTLRv3Tree_py.g:111:18: optionsSpec
                    pass 
                    self._state.following.append(self.FOLLOW_optionsSpec_in_block494)
                    self.optionsSpec()

                    self._state.following.pop()



                # /home/res/ANTLRv3Tree_py.g:111:31: ( alternative rewrite )+
                cnt23 = 0
                while True: #loop23
                    alt23 = 2
                    LA23_0 = self.input.LA(1)

                    if (LA23_0 == ALT) :
                        alt23 = 1


                    if alt23 == 1:
                        # /home/res/ANTLRv3Tree_py.g:111:32: alternative rewrite
                        pass 
                        self._state.following.append(self.FOLLOW_alternative_in_block498)
                        self.alternative()

                        self._state.following.pop()
                        self._state.following.append(self.FOLLOW_rewrite_in_block500)
                        self.rewrite()

                        self._state.following.pop()


                    else:
                        if cnt23 >= 1:
                            break #loop23

                        eee = EarlyExitException(23, self.input)
                        raise eee

                    cnt23 += 1


                self.match(self.input, EOB, self.FOLLOW_EOB_in_block504)

                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "block"


    # $ANTLR start "altList"
    # /home/res/ANTLRv3Tree_py.g:114:1: altList : ^( BLOCK ( alternative rewrite )+ EOB ) ;
    def altList(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:115:5: ( ^( BLOCK ( alternative rewrite )+ EOB ) )
                # /home/res/ANTLRv3Tree_py.g:115:9: ^( BLOCK ( alternative rewrite )+ EOB )
                pass 
                self.match(self.input, BLOCK, self.FOLLOW_BLOCK_in_altList527)

                self.match(self.input, DOWN, None)
                # /home/res/ANTLRv3Tree_py.g:115:18: ( alternative rewrite )+
                cnt24 = 0
                while True: #loop24
                    alt24 = 2
                    LA24_0 = self.input.LA(1)

                    if (LA24_0 == ALT) :
                        alt24 = 1


                    if alt24 == 1:
                        # /home/res/ANTLRv3Tree_py.g:115:19: alternative rewrite
                        pass 
                        self._state.following.append(self.FOLLOW_alternative_in_altList530)
                        self.alternative()

                        self._state.following.pop()
                        self._state.following.append(self.FOLLOW_rewrite_in_altList532)
                        self.rewrite()

                        self._state.following.pop()


                    else:
                        if cnt24 >= 1:
                            break #loop24

                        eee = EarlyExitException(24, self.input)
                        raise eee

                    cnt24 += 1


                self.match(self.input, EOB, self.FOLLOW_EOB_in_altList536)

                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "altList"


    # $ANTLR start "alternative"
    # /home/res/ANTLRv3Tree_py.g:118:1: alternative : ( ^( ALT ( element )+ EOA ) | ^( ALT EPSILON EOA ) );
    def alternative(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:119:5: ( ^( ALT ( element )+ EOA ) | ^( ALT EPSILON EOA ) )
                alt26 = 2
                LA26_0 = self.input.LA(1)

                if (LA26_0 == ALT) :
                    LA26_1 = self.input.LA(2)

                    if (LA26_1 == 2) :
                        LA26_2 = self.input.LA(3)

                        if (LA26_2 == EPSILON) :
                            alt26 = 2
                        elif ((BLOCK <= LA26_2 <= SYNPRED) or LA26_2 == CHAR_RANGE or (SEMPRED <= LA26_2 <= SYN_SEMPRED) or (TREE_BEGIN <= LA26_2 <= BANG) or (TOKEN_REF <= LA26_2 <= ACTION) or LA26_2 == RULE_REF or LA26_2 == 71 or LA26_2 == 87 or LA26_2 == 89 or LA26_2 == 92) :
                            alt26 = 1
                        else:
                            nvae = NoViableAltException("", 26, 2, self.input)

                            raise nvae

                    else:
                        nvae = NoViableAltException("", 26, 1, self.input)

                        raise nvae

                else:
                    nvae = NoViableAltException("", 26, 0, self.input)

                    raise nvae

                if alt26 == 1:
                    # /home/res/ANTLRv3Tree_py.g:119:9: ^( ALT ( element )+ EOA )
                    pass 
                    self.match(self.input, ALT, self.FOLLOW_ALT_in_alternative558)

                    self.match(self.input, DOWN, None)
                    # /home/res/ANTLRv3Tree_py.g:119:15: ( element )+
                    cnt25 = 0
                    while True: #loop25
                        alt25 = 2
                        LA25_0 = self.input.LA(1)

                        if ((BLOCK <= LA25_0 <= SYNPRED) or LA25_0 == CHAR_RANGE or (SEMPRED <= LA25_0 <= SYN_SEMPRED) or (TREE_BEGIN <= LA25_0 <= BANG) or (TOKEN_REF <= LA25_0 <= ACTION) or LA25_0 == RULE_REF or LA25_0 == 71 or LA25_0 == 87 or LA25_0 == 89 or LA25_0 == 92) :
                            alt25 = 1


                        if alt25 == 1:
                            # /home/res/ANTLRv3Tree_py.g:119:15: element
                            pass 
                            self._state.following.append(self.FOLLOW_element_in_alternative560)
                            self.element()

                            self._state.following.pop()


                        else:
                            if cnt25 >= 1:
                                break #loop25

                            eee = EarlyExitException(25, self.input)
                            raise eee

                        cnt25 += 1


                    self.match(self.input, EOA, self.FOLLOW_EOA_in_alternative563)

                    self.match(self.input, UP, None)


                elif alt26 == 2:
                    # /home/res/ANTLRv3Tree_py.g:120:9: ^( ALT EPSILON EOA )
                    pass 
                    self.match(self.input, ALT, self.FOLLOW_ALT_in_alternative575)

                    self.match(self.input, DOWN, None)
                    self.match(self.input, EPSILON, self.FOLLOW_EPSILON_in_alternative577)
                    self.match(self.input, EOA, self.FOLLOW_EOA_in_alternative579)

                    self.match(self.input, UP, None)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "alternative"


    # $ANTLR start "exceptionGroup"
    # /home/res/ANTLRv3Tree_py.g:123:1: exceptionGroup : ( ( exceptionHandler )+ ( finallyClause )? | finallyClause );
    def exceptionGroup(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:124:2: ( ( exceptionHandler )+ ( finallyClause )? | finallyClause )
                alt29 = 2
                LA29_0 = self.input.LA(1)

                if (LA29_0 == 85) :
                    alt29 = 1
                elif (LA29_0 == 86) :
                    alt29 = 2
                else:
                    nvae = NoViableAltException("", 29, 0, self.input)

                    raise nvae

                if alt29 == 1:
                    # /home/res/ANTLRv3Tree_py.g:124:4: ( exceptionHandler )+ ( finallyClause )?
                    pass 
                    # /home/res/ANTLRv3Tree_py.g:124:4: ( exceptionHandler )+
                    cnt27 = 0
                    while True: #loop27
                        alt27 = 2
                        LA27_0 = self.input.LA(1)

                        if (LA27_0 == 85) :
                            alt27 = 1


                        if alt27 == 1:
                            # /home/res/ANTLRv3Tree_py.g:124:4: exceptionHandler
                            pass 
                            self._state.following.append(self.FOLLOW_exceptionHandler_in_exceptionGroup594)
                            self.exceptionHandler()

                            self._state.following.pop()


                        else:
                            if cnt27 >= 1:
                                break #loop27

                            eee = EarlyExitException(27, self.input)
                            raise eee

                        cnt27 += 1


                    # /home/res/ANTLRv3Tree_py.g:124:22: ( finallyClause )?
                    alt28 = 2
                    LA28_0 = self.input.LA(1)

                    if (LA28_0 == 86) :
                        alt28 = 1
                    if alt28 == 1:
                        # /home/res/ANTLRv3Tree_py.g:124:22: finallyClause
                        pass 
                        self._state.following.append(self.FOLLOW_finallyClause_in_exceptionGroup597)
                        self.finallyClause()

                        self._state.following.pop()





                elif alt29 == 2:
                    # /home/res/ANTLRv3Tree_py.g:125:4: finallyClause
                    pass 
                    self._state.following.append(self.FOLLOW_finallyClause_in_exceptionGroup603)
                    self.finallyClause()

                    self._state.following.pop()



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "exceptionGroup"


    # $ANTLR start "exceptionHandler"
    # /home/res/ANTLRv3Tree_py.g:128:1: exceptionHandler : ^( 'catch' ARG_ACTION ACTION ) ;
    def exceptionHandler(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:129:5: ( ^( 'catch' ARG_ACTION ACTION ) )
                # /home/res/ANTLRv3Tree_py.g:129:10: ^( 'catch' ARG_ACTION ACTION )
                pass 
                self.match(self.input, 85, self.FOLLOW_85_in_exceptionHandler624)

                self.match(self.input, DOWN, None)
                self.match(self.input, ARG_ACTION, self.FOLLOW_ARG_ACTION_in_exceptionHandler626)
                self.match(self.input, ACTION, self.FOLLOW_ACTION_in_exceptionHandler628)

                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "exceptionHandler"


    # $ANTLR start "finallyClause"
    # /home/res/ANTLRv3Tree_py.g:132:1: finallyClause : ^( 'finally' ACTION ) ;
    def finallyClause(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:133:5: ( ^( 'finally' ACTION ) )
                # /home/res/ANTLRv3Tree_py.g:133:10: ^( 'finally' ACTION )
                pass 
                self.match(self.input, 86, self.FOLLOW_86_in_finallyClause650)

                self.match(self.input, DOWN, None)
                self.match(self.input, ACTION, self.FOLLOW_ACTION_in_finallyClause652)

                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "finallyClause"


    # $ANTLR start "element"
    # /home/res/ANTLRv3Tree_py.g:136:1: element : elementNoOptionSpec ;
    def element(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:137:2: ( elementNoOptionSpec )
                # /home/res/ANTLRv3Tree_py.g:137:4: elementNoOptionSpec
                pass 
                self._state.following.append(self.FOLLOW_elementNoOptionSpec_in_element667)
                self.elementNoOptionSpec()

                self._state.following.pop()




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "element"


    # $ANTLR start "elementNoOptionSpec"
    # /home/res/ANTLRv3Tree_py.g:140:1: elementNoOptionSpec : ( ^( ( '=' | '+=' ) ID block ) | ^( ( '=' | '+=' ) ID atom ) | atom | ebnf | ACTION | SEMPRED | GATED_SEMPRED | treeSpec );
    def elementNoOptionSpec(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:141:2: ( ^( ( '=' | '+=' ) ID block ) | ^( ( '=' | '+=' ) ID atom ) | atom | ebnf | ACTION | SEMPRED | GATED_SEMPRED | treeSpec )
                alt30 = 8
                alt30 = self.dfa30.predict(self.input)
                if alt30 == 1:
                    # /home/res/ANTLRv3Tree_py.g:141:4: ^( ( '=' | '+=' ) ID block )
                    pass 
                    if self.input.LA(1) == 71 or self.input.LA(1) == 87:
                        self.input.consume()
                        self._state.errorRecovery = False

                    else:
                        mse = MismatchedSetException(None, self.input)
                        raise mse



                    self.match(self.input, DOWN, None)
                    self.match(self.input, ID, self.FOLLOW_ID_in_elementNoOptionSpec685)
                    self._state.following.append(self.FOLLOW_block_in_elementNoOptionSpec687)
                    self.block()

                    self._state.following.pop()

                    self.match(self.input, UP, None)


                elif alt30 == 2:
                    # /home/res/ANTLRv3Tree_py.g:142:4: ^( ( '=' | '+=' ) ID atom )
                    pass 
                    if self.input.LA(1) == 71 or self.input.LA(1) == 87:
                        self.input.consume()
                        self._state.errorRecovery = False

                    else:
                        mse = MismatchedSetException(None, self.input)
                        raise mse



                    self.match(self.input, DOWN, None)
                    self.match(self.input, ID, self.FOLLOW_ID_in_elementNoOptionSpec700)
                    self._state.following.append(self.FOLLOW_atom_in_elementNoOptionSpec702)
                    self.atom()

                    self._state.following.pop()

                    self.match(self.input, UP, None)


                elif alt30 == 3:
                    # /home/res/ANTLRv3Tree_py.g:143:4: atom
                    pass 
                    self._state.following.append(self.FOLLOW_atom_in_elementNoOptionSpec708)
                    self.atom()

                    self._state.following.pop()


                elif alt30 == 4:
                    # /home/res/ANTLRv3Tree_py.g:144:4: ebnf
                    pass 
                    self._state.following.append(self.FOLLOW_ebnf_in_elementNoOptionSpec713)
                    self.ebnf()

                    self._state.following.pop()


                elif alt30 == 5:
                    # /home/res/ANTLRv3Tree_py.g:145:6: ACTION
                    pass 
                    self.match(self.input, ACTION, self.FOLLOW_ACTION_in_elementNoOptionSpec720)


                elif alt30 == 6:
                    # /home/res/ANTLRv3Tree_py.g:146:6: SEMPRED
                    pass 
                    self.match(self.input, SEMPRED, self.FOLLOW_SEMPRED_in_elementNoOptionSpec727)


                elif alt30 == 7:
                    # /home/res/ANTLRv3Tree_py.g:147:4: GATED_SEMPRED
                    pass 
                    self.match(self.input, GATED_SEMPRED, self.FOLLOW_GATED_SEMPRED_in_elementNoOptionSpec732)


                elif alt30 == 8:
                    # /home/res/ANTLRv3Tree_py.g:148:6: treeSpec
                    pass 
                    self._state.following.append(self.FOLLOW_treeSpec_in_elementNoOptionSpec739)
                    self.treeSpec()

                    self._state.following.pop()



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "elementNoOptionSpec"


    # $ANTLR start "atom"
    # /home/res/ANTLRv3Tree_py.g:151:1: atom : ( ^( ( '^' | '!' ) atom ) | range | notSet | ^( RULE_REF ARG_ACTION ) | RULE_REF | terminal );
    def atom(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:151:5: ( ^( ( '^' | '!' ) atom ) | range | notSet | ^( RULE_REF ARG_ACTION ) | RULE_REF | terminal )
                alt31 = 6
                LA31 = self.input.LA(1)
                if LA31 == ROOT or LA31 == BANG:
                    alt31 = 1
                elif LA31 == CHAR_RANGE:
                    alt31 = 2
                elif LA31 == 89:
                    alt31 = 3
                elif LA31 == RULE_REF:
                    LA31_4 = self.input.LA(2)

                    if (LA31_4 == 2) :
                        alt31 = 4
                    elif (LA31_4 == 3 or (BLOCK <= LA31_4 <= SYNPRED) or LA31_4 == CHAR_RANGE or LA31_4 == EOA or (SEMPRED <= LA31_4 <= SYN_SEMPRED) or (TREE_BEGIN <= LA31_4 <= BANG) or (TOKEN_REF <= LA31_4 <= ACTION) or LA31_4 == RULE_REF or LA31_4 == 71 or LA31_4 == 87 or LA31_4 == 89 or LA31_4 == 92) :
                        alt31 = 5
                    else:
                        nvae = NoViableAltException("", 31, 4, self.input)

                        raise nvae

                elif LA31 == TOKEN_REF or LA31 == STRING_LITERAL or LA31 == CHAR_LITERAL or LA31 == 92:
                    alt31 = 6
                else:
                    nvae = NoViableAltException("", 31, 0, self.input)

                    raise nvae

                if alt31 == 1:
                    # /home/res/ANTLRv3Tree_py.g:151:9: ^( ( '^' | '!' ) atom )
                    pass 
                    if (ROOT <= self.input.LA(1) <= BANG):
                        self.input.consume()
                        self._state.errorRecovery = False

                    else:
                        mse = MismatchedSetException(None, self.input)
                        raise mse



                    self.match(self.input, DOWN, None)
                    self._state.following.append(self.FOLLOW_atom_in_atom757)
                    self.atom()

                    self._state.following.pop()

                    self.match(self.input, UP, None)


                elif alt31 == 2:
                    # /home/res/ANTLRv3Tree_py.g:152:4: range
                    pass 
                    self._state.following.append(self.FOLLOW_range_in_atom763)
                    self.range()

                    self._state.following.pop()


                elif alt31 == 3:
                    # /home/res/ANTLRv3Tree_py.g:153:4: notSet
                    pass 
                    self._state.following.append(self.FOLLOW_notSet_in_atom768)
                    self.notSet()

                    self._state.following.pop()


                elif alt31 == 4:
                    # /home/res/ANTLRv3Tree_py.g:154:7: ^( RULE_REF ARG_ACTION )
                    pass 
                    self.match(self.input, RULE_REF, self.FOLLOW_RULE_REF_in_atom777)

                    self.match(self.input, DOWN, None)
                    self.match(self.input, ARG_ACTION, self.FOLLOW_ARG_ACTION_in_atom779)

                    self.match(self.input, UP, None)


                elif alt31 == 5:
                    # /home/res/ANTLRv3Tree_py.g:155:7: RULE_REF
                    pass 
                    self.match(self.input, RULE_REF, self.FOLLOW_RULE_REF_in_atom788)


                elif alt31 == 6:
                    # /home/res/ANTLRv3Tree_py.g:156:9: terminal
                    pass 
                    self._state.following.append(self.FOLLOW_terminal_in_atom798)
                    self.terminal()

                    self._state.following.pop()



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "atom"


    # $ANTLR start "notSet"
    # /home/res/ANTLRv3Tree_py.g:159:1: notSet : ( ^( '~' notTerminal ) | ^( '~' block ) );
    def notSet(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:160:2: ( ^( '~' notTerminal ) | ^( '~' block ) )
                alt32 = 2
                LA32_0 = self.input.LA(1)

                if (LA32_0 == 89) :
                    LA32_1 = self.input.LA(2)

                    if (LA32_1 == 2) :
                        LA32_2 = self.input.LA(3)

                        if (LA32_2 == BLOCK) :
                            alt32 = 2
                        elif ((TOKEN_REF <= LA32_2 <= CHAR_LITERAL)) :
                            alt32 = 1
                        else:
                            nvae = NoViableAltException("", 32, 2, self.input)

                            raise nvae

                    else:
                        nvae = NoViableAltException("", 32, 1, self.input)

                        raise nvae

                else:
                    nvae = NoViableAltException("", 32, 0, self.input)

                    raise nvae

                if alt32 == 1:
                    # /home/res/ANTLRv3Tree_py.g:160:4: ^( '~' notTerminal )
                    pass 
                    self.match(self.input, 89, self.FOLLOW_89_in_notSet813)

                    self.match(self.input, DOWN, None)
                    self._state.following.append(self.FOLLOW_notTerminal_in_notSet815)
                    self.notTerminal()

                    self._state.following.pop()

                    self.match(self.input, UP, None)


                elif alt32 == 2:
                    # /home/res/ANTLRv3Tree_py.g:161:4: ^( '~' block )
                    pass 
                    self.match(self.input, 89, self.FOLLOW_89_in_notSet822)

                    self.match(self.input, DOWN, None)
                    self._state.following.append(self.FOLLOW_block_in_notSet824)
                    self.block()

                    self._state.following.pop()

                    self.match(self.input, UP, None)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "notSet"


    # $ANTLR start "treeSpec"
    # /home/res/ANTLRv3Tree_py.g:164:1: treeSpec : ^( TREE_BEGIN ( element )+ ) ;
    def treeSpec(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:165:2: ( ^( TREE_BEGIN ( element )+ ) )
                # /home/res/ANTLRv3Tree_py.g:165:4: ^( TREE_BEGIN ( element )+ )
                pass 
                self.match(self.input, TREE_BEGIN, self.FOLLOW_TREE_BEGIN_in_treeSpec837)

                self.match(self.input, DOWN, None)
                # /home/res/ANTLRv3Tree_py.g:165:17: ( element )+
                cnt33 = 0
                while True: #loop33
                    alt33 = 2
                    LA33_0 = self.input.LA(1)

                    if ((BLOCK <= LA33_0 <= SYNPRED) or LA33_0 == CHAR_RANGE or (SEMPRED <= LA33_0 <= SYN_SEMPRED) or (TREE_BEGIN <= LA33_0 <= BANG) or (TOKEN_REF <= LA33_0 <= ACTION) or LA33_0 == RULE_REF or LA33_0 == 71 or LA33_0 == 87 or LA33_0 == 89 or LA33_0 == 92) :
                        alt33 = 1


                    if alt33 == 1:
                        # /home/res/ANTLRv3Tree_py.g:165:17: element
                        pass 
                        self._state.following.append(self.FOLLOW_element_in_treeSpec839)
                        self.element()

                        self._state.following.pop()


                    else:
                        if cnt33 >= 1:
                            break #loop33

                        eee = EarlyExitException(33, self.input)
                        raise eee

                    cnt33 += 1



                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "treeSpec"


    # $ANTLR start "ebnf"
    # /home/res/ANTLRv3Tree_py.g:168:1: ebnf : ( ^( SYNPRED block ) | SYN_SEMPRED | ^( ebnfSuffix block ) | block );
    def ebnf(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:170:2: ( ^( SYNPRED block ) | SYN_SEMPRED | ^( ebnfSuffix block ) | block )
                alt34 = 4
                LA34 = self.input.LA(1)
                if LA34 == SYNPRED:
                    alt34 = 1
                elif LA34 == SYN_SEMPRED:
                    alt34 = 2
                elif LA34 == OPTIONAL or LA34 == CLOSURE or LA34 == POSITIVE_CLOSURE:
                    alt34 = 3
                elif LA34 == BLOCK:
                    alt34 = 4
                else:
                    nvae = NoViableAltException("", 34, 0, self.input)

                    raise nvae

                if alt34 == 1:
                    # /home/res/ANTLRv3Tree_py.g:170:4: ^( SYNPRED block )
                    pass 
                    self.match(self.input, SYNPRED, self.FOLLOW_SYNPRED_in_ebnf855)

                    self.match(self.input, DOWN, None)
                    self._state.following.append(self.FOLLOW_block_in_ebnf857)
                    self.block()

                    self._state.following.pop()

                    self.match(self.input, UP, None)


                elif alt34 == 2:
                    # /home/res/ANTLRv3Tree_py.g:171:4: SYN_SEMPRED
                    pass 
                    self.match(self.input, SYN_SEMPRED, self.FOLLOW_SYN_SEMPRED_in_ebnf863)


                elif alt34 == 3:
                    # /home/res/ANTLRv3Tree_py.g:172:4: ^( ebnfSuffix block )
                    pass 
                    self._state.following.append(self.FOLLOW_ebnfSuffix_in_ebnf869)
                    self.ebnfSuffix()

                    self._state.following.pop()

                    self.match(self.input, DOWN, None)
                    self._state.following.append(self.FOLLOW_block_in_ebnf871)
                    self.block()

                    self._state.following.pop()

                    self.match(self.input, UP, None)


                elif alt34 == 4:
                    # /home/res/ANTLRv3Tree_py.g:173:4: block
                    pass 
                    self._state.following.append(self.FOLLOW_block_in_ebnf877)
                    self.block()

                    self._state.following.pop()



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "ebnf"


    # $ANTLR start "range"
    # /home/res/ANTLRv3Tree_py.g:176:1: range : ^( CHAR_RANGE CHAR_LITERAL CHAR_LITERAL ) ;
    def range(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:177:2: ( ^( CHAR_RANGE CHAR_LITERAL CHAR_LITERAL ) )
                # /home/res/ANTLRv3Tree_py.g:177:4: ^( CHAR_RANGE CHAR_LITERAL CHAR_LITERAL )
                pass 
                self.match(self.input, CHAR_RANGE, self.FOLLOW_CHAR_RANGE_in_range889)

                self.match(self.input, DOWN, None)
                self.match(self.input, CHAR_LITERAL, self.FOLLOW_CHAR_LITERAL_in_range891)
                self.match(self.input, CHAR_LITERAL, self.FOLLOW_CHAR_LITERAL_in_range893)

                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "range"


    # $ANTLR start "terminal"
    # /home/res/ANTLRv3Tree_py.g:180:1: terminal : ( CHAR_LITERAL | TOKEN_REF | STRING_LITERAL | ^( TOKEN_REF ARG_ACTION ) | '.' );
    def terminal(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:181:5: ( CHAR_LITERAL | TOKEN_REF | STRING_LITERAL | ^( TOKEN_REF ARG_ACTION ) | '.' )
                alt35 = 5
                LA35 = self.input.LA(1)
                if LA35 == CHAR_LITERAL:
                    alt35 = 1
                elif LA35 == TOKEN_REF:
                    LA35_2 = self.input.LA(2)

                    if (LA35_2 == 2) :
                        alt35 = 4
                    elif (LA35_2 == 3 or (BLOCK <= LA35_2 <= SYNPRED) or LA35_2 == CHAR_RANGE or LA35_2 == EOA or (SEMPRED <= LA35_2 <= SYN_SEMPRED) or (TREE_BEGIN <= LA35_2 <= BANG) or (TOKEN_REF <= LA35_2 <= ACTION) or LA35_2 == RULE_REF or LA35_2 == 71 or LA35_2 == 87 or LA35_2 == 89 or LA35_2 == 92) :
                        alt35 = 2
                    else:
                        nvae = NoViableAltException("", 35, 2, self.input)

                        raise nvae

                elif LA35 == STRING_LITERAL:
                    alt35 = 3
                elif LA35 == 92:
                    alt35 = 5
                else:
                    nvae = NoViableAltException("", 35, 0, self.input)

                    raise nvae

                if alt35 == 1:
                    # /home/res/ANTLRv3Tree_py.g:181:9: CHAR_LITERAL
                    pass 
                    self.match(self.input, CHAR_LITERAL, self.FOLLOW_CHAR_LITERAL_in_terminal910)


                elif alt35 == 2:
                    # /home/res/ANTLRv3Tree_py.g:182:7: TOKEN_REF
                    pass 
                    self.match(self.input, TOKEN_REF, self.FOLLOW_TOKEN_REF_in_terminal918)


                elif alt35 == 3:
                    # /home/res/ANTLRv3Tree_py.g:183:7: STRING_LITERAL
                    pass 
                    self.match(self.input, STRING_LITERAL, self.FOLLOW_STRING_LITERAL_in_terminal926)


                elif alt35 == 4:
                    # /home/res/ANTLRv3Tree_py.g:184:7: ^( TOKEN_REF ARG_ACTION )
                    pass 
                    self.match(self.input, TOKEN_REF, self.FOLLOW_TOKEN_REF_in_terminal935)

                    self.match(self.input, DOWN, None)
                    self.match(self.input, ARG_ACTION, self.FOLLOW_ARG_ACTION_in_terminal937)

                    self.match(self.input, UP, None)


                elif alt35 == 5:
                    # /home/res/ANTLRv3Tree_py.g:185:7: '.'
                    pass 
                    self.match(self.input, 92, self.FOLLOW_92_in_terminal946)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "terminal"


    # $ANTLR start "notTerminal"
    # /home/res/ANTLRv3Tree_py.g:188:1: notTerminal : ( CHAR_LITERAL | TOKEN_REF | STRING_LITERAL );
    def notTerminal(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:189:2: ( CHAR_LITERAL | TOKEN_REF | STRING_LITERAL )
                # /home/res/ANTLRv3Tree_py.g:
                pass 
                if (TOKEN_REF <= self.input.LA(1) <= CHAR_LITERAL):
                    self.input.consume()
                    self._state.errorRecovery = False

                else:
                    mse = MismatchedSetException(None, self.input)
                    raise mse






            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "notTerminal"


    # $ANTLR start "ebnfSuffix"
    # /home/res/ANTLRv3Tree_py.g:194:1: ebnfSuffix : ( OPTIONAL | CLOSURE | POSITIVE_CLOSURE );
    def ebnfSuffix(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:195:2: ( OPTIONAL | CLOSURE | POSITIVE_CLOSURE )
                # /home/res/ANTLRv3Tree_py.g:
                pass 
                if (OPTIONAL <= self.input.LA(1) <= POSITIVE_CLOSURE):
                    self.input.consume()
                    self._state.errorRecovery = False

                else:
                    mse = MismatchedSetException(None, self.input)
                    raise mse






            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "ebnfSuffix"


    # $ANTLR start "rewrite"
    # /home/res/ANTLRv3Tree_py.g:202:1: rewrite : ( ( ^( '->' SEMPRED rewrite_alternative ) )* ^( '->' rewrite_alternative ) | );
    def rewrite(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:203:2: ( ( ^( '->' SEMPRED rewrite_alternative ) )* ^( '->' rewrite_alternative ) | )
                alt37 = 2
                LA37_0 = self.input.LA(1)

                if (LA37_0 == REWRITE) :
                    alt37 = 1
                elif (LA37_0 == ALT or LA37_0 == EOB) :
                    alt37 = 2
                else:
                    nvae = NoViableAltException("", 37, 0, self.input)

                    raise nvae

                if alt37 == 1:
                    # /home/res/ANTLRv3Tree_py.g:203:4: ( ^( '->' SEMPRED rewrite_alternative ) )* ^( '->' rewrite_alternative )
                    pass 
                    # /home/res/ANTLRv3Tree_py.g:203:4: ( ^( '->' SEMPRED rewrite_alternative ) )*
                    while True: #loop36
                        alt36 = 2
                        LA36_0 = self.input.LA(1)

                        if (LA36_0 == REWRITE) :
                            LA36_1 = self.input.LA(2)

                            if (LA36_1 == 2) :
                                LA36_2 = self.input.LA(3)

                                if (LA36_2 == SEMPRED) :
                                    alt36 = 1






                        if alt36 == 1:
                            # /home/res/ANTLRv3Tree_py.g:203:5: ^( '->' SEMPRED rewrite_alternative )
                            pass 
                            self.match(self.input, REWRITE, self.FOLLOW_REWRITE_in_rewrite1012)

                            self.match(self.input, DOWN, None)
                            self.match(self.input, SEMPRED, self.FOLLOW_SEMPRED_in_rewrite1014)
                            self._state.following.append(self.FOLLOW_rewrite_alternative_in_rewrite1016)
                            self.rewrite_alternative()

                            self._state.following.pop()

                            self.match(self.input, UP, None)


                        else:
                            break #loop36


                    self.match(self.input, REWRITE, self.FOLLOW_REWRITE_in_rewrite1022)

                    self.match(self.input, DOWN, None)
                    self._state.following.append(self.FOLLOW_rewrite_alternative_in_rewrite1024)
                    self.rewrite_alternative()

                    self._state.following.pop()

                    self.match(self.input, UP, None)


                elif alt37 == 2:
                    # /home/res/ANTLRv3Tree_py.g:205:2: 
                    pass 


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "rewrite"


    # $ANTLR start "rewrite_alternative"
    # /home/res/ANTLRv3Tree_py.g:207:1: rewrite_alternative : ( rewrite_template | rewrite_tree_alternative | ^( ALT EPSILON EOA ) );
    def rewrite_alternative(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:208:2: ( rewrite_template | rewrite_tree_alternative | ^( ALT EPSILON EOA ) )
                alt38 = 3
                LA38_0 = self.input.LA(1)

                if (LA38_0 == TEMPLATE or LA38_0 == ACTION) :
                    alt38 = 1
                elif (LA38_0 == ALT) :
                    LA38_2 = self.input.LA(2)

                    if (LA38_2 == 2) :
                        LA38_3 = self.input.LA(3)

                        if (LA38_3 == EPSILON) :
                            alt38 = 3
                        elif ((BLOCK <= LA38_3 <= POSITIVE_CLOSURE) or LA38_3 == LABEL or LA38_3 == TREE_BEGIN or (TOKEN_REF <= LA38_3 <= ACTION) or LA38_3 == RULE_REF) :
                            alt38 = 2
                        else:
                            nvae = NoViableAltException("", 38, 3, self.input)

                            raise nvae

                    else:
                        nvae = NoViableAltException("", 38, 2, self.input)

                        raise nvae

                else:
                    nvae = NoViableAltException("", 38, 0, self.input)

                    raise nvae

                if alt38 == 1:
                    # /home/res/ANTLRv3Tree_py.g:208:4: rewrite_template
                    pass 
                    self._state.following.append(self.FOLLOW_rewrite_template_in_rewrite_alternative1039)
                    self.rewrite_template()

                    self._state.following.pop()


                elif alt38 == 2:
                    # /home/res/ANTLRv3Tree_py.g:209:4: rewrite_tree_alternative
                    pass 
                    self._state.following.append(self.FOLLOW_rewrite_tree_alternative_in_rewrite_alternative1044)
                    self.rewrite_tree_alternative()

                    self._state.following.pop()


                elif alt38 == 3:
                    # /home/res/ANTLRv3Tree_py.g:210:9: ^( ALT EPSILON EOA )
                    pass 
                    self.match(self.input, ALT, self.FOLLOW_ALT_in_rewrite_alternative1055)

                    self.match(self.input, DOWN, None)
                    self.match(self.input, EPSILON, self.FOLLOW_EPSILON_in_rewrite_alternative1057)
                    self.match(self.input, EOA, self.FOLLOW_EOA_in_rewrite_alternative1059)

                    self.match(self.input, UP, None)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "rewrite_alternative"


    # $ANTLR start "rewrite_tree_block"
    # /home/res/ANTLRv3Tree_py.g:213:1: rewrite_tree_block : ^( BLOCK rewrite_tree_alternative EOB ) ;
    def rewrite_tree_block(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:214:5: ( ^( BLOCK rewrite_tree_alternative EOB ) )
                # /home/res/ANTLRv3Tree_py.g:214:9: ^( BLOCK rewrite_tree_alternative EOB )
                pass 
                self.match(self.input, BLOCK, self.FOLLOW_BLOCK_in_rewrite_tree_block1078)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_rewrite_tree_alternative_in_rewrite_tree_block1080)
                self.rewrite_tree_alternative()

                self._state.following.pop()
                self.match(self.input, EOB, self.FOLLOW_EOB_in_rewrite_tree_block1082)

                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "rewrite_tree_block"


    # $ANTLR start "rewrite_tree_alternative"
    # /home/res/ANTLRv3Tree_py.g:217:1: rewrite_tree_alternative : ^( ALT ( rewrite_tree_element )+ EOA ) ;
    def rewrite_tree_alternative(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:218:5: ( ^( ALT ( rewrite_tree_element )+ EOA ) )
                # /home/res/ANTLRv3Tree_py.g:218:7: ^( ALT ( rewrite_tree_element )+ EOA )
                pass 
                self.match(self.input, ALT, self.FOLLOW_ALT_in_rewrite_tree_alternative1101)

                self.match(self.input, DOWN, None)
                # /home/res/ANTLRv3Tree_py.g:218:13: ( rewrite_tree_element )+
                cnt39 = 0
                while True: #loop39
                    alt39 = 2
                    LA39_0 = self.input.LA(1)

                    if ((BLOCK <= LA39_0 <= POSITIVE_CLOSURE) or LA39_0 == LABEL or LA39_0 == TREE_BEGIN or (TOKEN_REF <= LA39_0 <= ACTION) or LA39_0 == RULE_REF) :
                        alt39 = 1


                    if alt39 == 1:
                        # /home/res/ANTLRv3Tree_py.g:218:13: rewrite_tree_element
                        pass 
                        self._state.following.append(self.FOLLOW_rewrite_tree_element_in_rewrite_tree_alternative1103)
                        self.rewrite_tree_element()

                        self._state.following.pop()


                    else:
                        if cnt39 >= 1:
                            break #loop39

                        eee = EarlyExitException(39, self.input)
                        raise eee

                    cnt39 += 1


                self.match(self.input, EOA, self.FOLLOW_EOA_in_rewrite_tree_alternative1106)

                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "rewrite_tree_alternative"


    # $ANTLR start "rewrite_tree_element"
    # /home/res/ANTLRv3Tree_py.g:221:1: rewrite_tree_element : ( rewrite_tree_atom | rewrite_tree | rewrite_tree_block | rewrite_tree_ebnf );
    def rewrite_tree_element(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:222:2: ( rewrite_tree_atom | rewrite_tree | rewrite_tree_block | rewrite_tree_ebnf )
                alt40 = 4
                LA40 = self.input.LA(1)
                if LA40 == LABEL or LA40 == TOKEN_REF or LA40 == STRING_LITERAL or LA40 == CHAR_LITERAL or LA40 == ACTION or LA40 == RULE_REF:
                    alt40 = 1
                elif LA40 == TREE_BEGIN:
                    alt40 = 2
                elif LA40 == BLOCK:
                    alt40 = 3
                elif LA40 == OPTIONAL or LA40 == CLOSURE or LA40 == POSITIVE_CLOSURE:
                    alt40 = 4
                else:
                    nvae = NoViableAltException("", 40, 0, self.input)

                    raise nvae

                if alt40 == 1:
                    # /home/res/ANTLRv3Tree_py.g:222:4: rewrite_tree_atom
                    pass 
                    self._state.following.append(self.FOLLOW_rewrite_tree_atom_in_rewrite_tree_element1121)
                    self.rewrite_tree_atom()

                    self._state.following.pop()


                elif alt40 == 2:
                    # /home/res/ANTLRv3Tree_py.g:223:4: rewrite_tree
                    pass 
                    self._state.following.append(self.FOLLOW_rewrite_tree_in_rewrite_tree_element1126)
                    self.rewrite_tree()

                    self._state.following.pop()


                elif alt40 == 3:
                    # /home/res/ANTLRv3Tree_py.g:224:6: rewrite_tree_block
                    pass 
                    self._state.following.append(self.FOLLOW_rewrite_tree_block_in_rewrite_tree_element1133)
                    self.rewrite_tree_block()

                    self._state.following.pop()


                elif alt40 == 4:
                    # /home/res/ANTLRv3Tree_py.g:225:6: rewrite_tree_ebnf
                    pass 
                    self._state.following.append(self.FOLLOW_rewrite_tree_ebnf_in_rewrite_tree_element1140)
                    self.rewrite_tree_ebnf()

                    self._state.following.pop()



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "rewrite_tree_element"


    # $ANTLR start "rewrite_tree_atom"
    # /home/res/ANTLRv3Tree_py.g:228:1: rewrite_tree_atom : ( CHAR_LITERAL | TOKEN_REF | ^( TOKEN_REF ARG_ACTION ) | RULE_REF | STRING_LITERAL | LABEL | ACTION );
    def rewrite_tree_atom(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:229:5: ( CHAR_LITERAL | TOKEN_REF | ^( TOKEN_REF ARG_ACTION ) | RULE_REF | STRING_LITERAL | LABEL | ACTION )
                alt41 = 7
                LA41 = self.input.LA(1)
                if LA41 == CHAR_LITERAL:
                    alt41 = 1
                elif LA41 == TOKEN_REF:
                    LA41_2 = self.input.LA(2)

                    if (LA41_2 == 2) :
                        alt41 = 3
                    elif (LA41_2 == 3 or (BLOCK <= LA41_2 <= POSITIVE_CLOSURE) or LA41_2 == EOA or LA41_2 == LABEL or LA41_2 == TREE_BEGIN or (TOKEN_REF <= LA41_2 <= ACTION) or LA41_2 == RULE_REF) :
                        alt41 = 2
                    else:
                        nvae = NoViableAltException("", 41, 2, self.input)

                        raise nvae

                elif LA41 == RULE_REF:
                    alt41 = 4
                elif LA41 == STRING_LITERAL:
                    alt41 = 5
                elif LA41 == LABEL:
                    alt41 = 6
                elif LA41 == ACTION:
                    alt41 = 7
                else:
                    nvae = NoViableAltException("", 41, 0, self.input)

                    raise nvae

                if alt41 == 1:
                    # /home/res/ANTLRv3Tree_py.g:229:9: CHAR_LITERAL
                    pass 
                    self.match(self.input, CHAR_LITERAL, self.FOLLOW_CHAR_LITERAL_in_rewrite_tree_atom1156)


                elif alt41 == 2:
                    # /home/res/ANTLRv3Tree_py.g:230:6: TOKEN_REF
                    pass 
                    self.match(self.input, TOKEN_REF, self.FOLLOW_TOKEN_REF_in_rewrite_tree_atom1163)


                elif alt41 == 3:
                    # /home/res/ANTLRv3Tree_py.g:231:6: ^( TOKEN_REF ARG_ACTION )
                    pass 
                    self.match(self.input, TOKEN_REF, self.FOLLOW_TOKEN_REF_in_rewrite_tree_atom1171)

                    self.match(self.input, DOWN, None)
                    self.match(self.input, ARG_ACTION, self.FOLLOW_ARG_ACTION_in_rewrite_tree_atom1173)

                    self.match(self.input, UP, None)


                elif alt41 == 4:
                    # /home/res/ANTLRv3Tree_py.g:232:9: RULE_REF
                    pass 
                    self.match(self.input, RULE_REF, self.FOLLOW_RULE_REF_in_rewrite_tree_atom1185)


                elif alt41 == 5:
                    # /home/res/ANTLRv3Tree_py.g:233:6: STRING_LITERAL
                    pass 
                    self.match(self.input, STRING_LITERAL, self.FOLLOW_STRING_LITERAL_in_rewrite_tree_atom1192)


                elif alt41 == 6:
                    # /home/res/ANTLRv3Tree_py.g:234:6: LABEL
                    pass 
                    self.match(self.input, LABEL, self.FOLLOW_LABEL_in_rewrite_tree_atom1199)


                elif alt41 == 7:
                    # /home/res/ANTLRv3Tree_py.g:235:4: ACTION
                    pass 
                    self.match(self.input, ACTION, self.FOLLOW_ACTION_in_rewrite_tree_atom1204)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "rewrite_tree_atom"


    # $ANTLR start "rewrite_tree_ebnf"
    # /home/res/ANTLRv3Tree_py.g:238:1: rewrite_tree_ebnf : ^( ebnfSuffix rewrite_tree_block ) ;
    def rewrite_tree_ebnf(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:239:2: ( ^( ebnfSuffix rewrite_tree_block ) )
                # /home/res/ANTLRv3Tree_py.g:239:4: ^( ebnfSuffix rewrite_tree_block )
                pass 
                self._state.following.append(self.FOLLOW_ebnfSuffix_in_rewrite_tree_ebnf1216)
                self.ebnfSuffix()

                self._state.following.pop()

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_rewrite_tree_block_in_rewrite_tree_ebnf1218)
                self.rewrite_tree_block()

                self._state.following.pop()

                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "rewrite_tree_ebnf"


    # $ANTLR start "rewrite_tree"
    # /home/res/ANTLRv3Tree_py.g:242:1: rewrite_tree : ^( TREE_BEGIN rewrite_tree_atom ( rewrite_tree_element )* ) ;
    def rewrite_tree(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:243:2: ( ^( TREE_BEGIN rewrite_tree_atom ( rewrite_tree_element )* ) )
                # /home/res/ANTLRv3Tree_py.g:243:4: ^( TREE_BEGIN rewrite_tree_atom ( rewrite_tree_element )* )
                pass 
                self.match(self.input, TREE_BEGIN, self.FOLLOW_TREE_BEGIN_in_rewrite_tree1232)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_rewrite_tree_atom_in_rewrite_tree1234)
                self.rewrite_tree_atom()

                self._state.following.pop()
                # /home/res/ANTLRv3Tree_py.g:243:35: ( rewrite_tree_element )*
                while True: #loop42
                    alt42 = 2
                    LA42_0 = self.input.LA(1)

                    if ((BLOCK <= LA42_0 <= POSITIVE_CLOSURE) or LA42_0 == LABEL or LA42_0 == TREE_BEGIN or (TOKEN_REF <= LA42_0 <= ACTION) or LA42_0 == RULE_REF) :
                        alt42 = 1


                    if alt42 == 1:
                        # /home/res/ANTLRv3Tree_py.g:243:35: rewrite_tree_element
                        pass 
                        self._state.following.append(self.FOLLOW_rewrite_tree_element_in_rewrite_tree1236)
                        self.rewrite_tree_element()

                        self._state.following.pop()


                    else:
                        break #loop42



                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "rewrite_tree"


    # $ANTLR start "rewrite_template"
    # /home/res/ANTLRv3Tree_py.g:246:1: rewrite_template : ( ^( TEMPLATE ID rewrite_template_args ( DOUBLE_QUOTE_STRING_LITERAL | DOUBLE_ANGLE_STRING_LITERAL ) ) | rewrite_template_ref | rewrite_indirect_template_head | ACTION );
    def rewrite_template(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:247:2: ( ^( TEMPLATE ID rewrite_template_args ( DOUBLE_QUOTE_STRING_LITERAL | DOUBLE_ANGLE_STRING_LITERAL ) ) | rewrite_template_ref | rewrite_indirect_template_head | ACTION )
                alt43 = 4
                alt43 = self.dfa43.predict(self.input)
                if alt43 == 1:
                    # /home/res/ANTLRv3Tree_py.g:247:6: ^( TEMPLATE ID rewrite_template_args ( DOUBLE_QUOTE_STRING_LITERAL | DOUBLE_ANGLE_STRING_LITERAL ) )
                    pass 
                    self.match(self.input, TEMPLATE, self.FOLLOW_TEMPLATE_in_rewrite_template1254)

                    self.match(self.input, DOWN, None)
                    self.match(self.input, ID, self.FOLLOW_ID_in_rewrite_template1256)
                    self._state.following.append(self.FOLLOW_rewrite_template_args_in_rewrite_template1258)
                    self.rewrite_template_args()

                    self._state.following.pop()
                    if (DOUBLE_QUOTE_STRING_LITERAL <= self.input.LA(1) <= DOUBLE_ANGLE_STRING_LITERAL):
                        self.input.consume()
                        self._state.errorRecovery = False

                    else:
                        mse = MismatchedSetException(None, self.input)
                        raise mse



                    self.match(self.input, UP, None)


                elif alt43 == 2:
                    # /home/res/ANTLRv3Tree_py.g:250:4: rewrite_template_ref
                    pass 
                    self._state.following.append(self.FOLLOW_rewrite_template_ref_in_rewrite_template1281)
                    self.rewrite_template_ref()

                    self._state.following.pop()


                elif alt43 == 3:
                    # /home/res/ANTLRv3Tree_py.g:251:4: rewrite_indirect_template_head
                    pass 
                    self._state.following.append(self.FOLLOW_rewrite_indirect_template_head_in_rewrite_template1286)
                    self.rewrite_indirect_template_head()

                    self._state.following.pop()


                elif alt43 == 4:
                    # /home/res/ANTLRv3Tree_py.g:252:4: ACTION
                    pass 
                    self.match(self.input, ACTION, self.FOLLOW_ACTION_in_rewrite_template1291)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "rewrite_template"


    # $ANTLR start "rewrite_template_ref"
    # /home/res/ANTLRv3Tree_py.g:255:1: rewrite_template_ref : ^( TEMPLATE ID rewrite_template_args ) ;
    def rewrite_template_ref(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:257:2: ( ^( TEMPLATE ID rewrite_template_args ) )
                # /home/res/ANTLRv3Tree_py.g:257:4: ^( TEMPLATE ID rewrite_template_args )
                pass 
                self.match(self.input, TEMPLATE, self.FOLLOW_TEMPLATE_in_rewrite_template_ref1305)

                self.match(self.input, DOWN, None)
                self.match(self.input, ID, self.FOLLOW_ID_in_rewrite_template_ref1307)
                self._state.following.append(self.FOLLOW_rewrite_template_args_in_rewrite_template_ref1309)
                self.rewrite_template_args()

                self._state.following.pop()

                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "rewrite_template_ref"


    # $ANTLR start "rewrite_indirect_template_head"
    # /home/res/ANTLRv3Tree_py.g:260:1: rewrite_indirect_template_head : ^( TEMPLATE ACTION rewrite_template_args ) ;
    def rewrite_indirect_template_head(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:262:2: ( ^( TEMPLATE ACTION rewrite_template_args ) )
                # /home/res/ANTLRv3Tree_py.g:262:4: ^( TEMPLATE ACTION rewrite_template_args )
                pass 
                self.match(self.input, TEMPLATE, self.FOLLOW_TEMPLATE_in_rewrite_indirect_template_head1324)

                self.match(self.input, DOWN, None)
                self.match(self.input, ACTION, self.FOLLOW_ACTION_in_rewrite_indirect_template_head1326)
                self._state.following.append(self.FOLLOW_rewrite_template_args_in_rewrite_indirect_template_head1328)
                self.rewrite_template_args()

                self._state.following.pop()

                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "rewrite_indirect_template_head"


    # $ANTLR start "rewrite_template_args"
    # /home/res/ANTLRv3Tree_py.g:265:1: rewrite_template_args : ( ^( ARGLIST ( rewrite_template_arg )+ ) | ARGLIST );
    def rewrite_template_args(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:266:2: ( ^( ARGLIST ( rewrite_template_arg )+ ) | ARGLIST )
                alt45 = 2
                LA45_0 = self.input.LA(1)

                if (LA45_0 == ARGLIST) :
                    LA45_1 = self.input.LA(2)

                    if (LA45_1 == 2) :
                        alt45 = 1
                    elif (LA45_1 == 3 or (DOUBLE_QUOTE_STRING_LITERAL <= LA45_1 <= DOUBLE_ANGLE_STRING_LITERAL)) :
                        alt45 = 2
                    else:
                        nvae = NoViableAltException("", 45, 1, self.input)

                        raise nvae

                else:
                    nvae = NoViableAltException("", 45, 0, self.input)

                    raise nvae

                if alt45 == 1:
                    # /home/res/ANTLRv3Tree_py.g:266:4: ^( ARGLIST ( rewrite_template_arg )+ )
                    pass 
                    self.match(self.input, ARGLIST, self.FOLLOW_ARGLIST_in_rewrite_template_args1341)

                    self.match(self.input, DOWN, None)
                    # /home/res/ANTLRv3Tree_py.g:266:14: ( rewrite_template_arg )+
                    cnt44 = 0
                    while True: #loop44
                        alt44 = 2
                        LA44_0 = self.input.LA(1)

                        if (LA44_0 == ARG) :
                            alt44 = 1


                        if alt44 == 1:
                            # /home/res/ANTLRv3Tree_py.g:266:14: rewrite_template_arg
                            pass 
                            self._state.following.append(self.FOLLOW_rewrite_template_arg_in_rewrite_template_args1343)
                            self.rewrite_template_arg()

                            self._state.following.pop()


                        else:
                            if cnt44 >= 1:
                                break #loop44

                            eee = EarlyExitException(44, self.input)
                            raise eee

                        cnt44 += 1



                    self.match(self.input, UP, None)


                elif alt45 == 2:
                    # /home/res/ANTLRv3Tree_py.g:267:4: ARGLIST
                    pass 
                    self.match(self.input, ARGLIST, self.FOLLOW_ARGLIST_in_rewrite_template_args1350)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "rewrite_template_args"


    # $ANTLR start "rewrite_template_arg"
    # /home/res/ANTLRv3Tree_py.g:270:1: rewrite_template_arg : ^( ARG ID ACTION ) ;
    def rewrite_template_arg(self, ):

        try:
            try:
                # /home/res/ANTLRv3Tree_py.g:271:2: ( ^( ARG ID ACTION ) )
                # /home/res/ANTLRv3Tree_py.g:271:6: ^( ARG ID ACTION )
                pass 
                self.match(self.input, ARG, self.FOLLOW_ARG_in_rewrite_template_arg1364)

                self.match(self.input, DOWN, None)
                self.match(self.input, ID, self.FOLLOW_ID_in_rewrite_template_arg1366)
                self.match(self.input, ACTION, self.FOLLOW_ACTION_in_rewrite_template_arg1368)

                self.match(self.input, UP, None)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
        finally:

            pass

        return 

    # $ANTLR end "rewrite_template_arg"


    # Delegated rules


    # lookup tables for DFA #30

    DFA30_eot = DFA.unpack(
        u"\14\uffff"
        )

    DFA30_eof = DFA.unpack(
        u"\14\uffff"
        )

    DFA30_min = DFA.unpack(
        u"\1\10\1\2\6\uffff\1\24\1\10\2\uffff"
        )

    DFA30_max = DFA.unpack(
        u"\1\134\1\2\6\uffff\1\24\1\134\2\uffff"
        )

    DFA30_accept = DFA.unpack(
        u"\2\uffff\1\3\1\4\1\5\1\6\1\7\1\10\2\uffff\1\2\1\1"
        )

    DFA30_special = DFA.unpack(
        u"\14\uffff"
        )

            
    DFA30_transition = [
        DFA.unpack(u"\5\3\1\uffff\1\2\21\uffff\1\5\1\6\1\3\2\uffff\1\7\2"
        u"\2\2\uffff\3\2\1\4\3\uffff\1\2\25\uffff\1\1\17\uffff\1\1\1\uffff"
        u"\1\2\2\uffff\1\2"),
        DFA.unpack(u"\1\10"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\11"),
        DFA.unpack(u"\1\13\5\uffff\1\12\27\uffff\2\12\2\uffff\3\12\4\uffff"
        u"\1\12\47\uffff\1\12\2\uffff\1\12"),
        DFA.unpack(u""),
        DFA.unpack(u"")
    ]

    # class definition for DFA #30

    DFA30 = DFA
    # lookup tables for DFA #43

    DFA43_eot = DFA.unpack(
        u"\20\uffff"
        )

    DFA43_eof = DFA.unpack(
        u"\20\uffff"
        )

    DFA43_min = DFA.unpack(
        u"\1\36\1\2\1\uffff\1\24\1\26\1\uffff\1\2\1\25\2\uffff\1\2\1\24\1"
        u"\55\3\3"
        )

    DFA43_max = DFA.unpack(
        u"\1\55\1\2\1\uffff\1\55\1\26\1\uffff\1\63\1\25\2\uffff\1\2\1\24"
        u"\1\55\1\3\1\25\1\63"
        )

    DFA43_accept = DFA.unpack(
        u"\2\uffff\1\4\2\uffff\1\3\2\uffff\1\2\1\1\6\uffff"
        )

    DFA43_special = DFA.unpack(
        u"\20\uffff"
        )

            
    DFA43_transition = [
        DFA.unpack(u"\1\1\16\uffff\1\2"),
        DFA.unpack(u"\1\3"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\4\30\uffff\1\5"),
        DFA.unpack(u"\1\6"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\7\1\10\56\uffff\2\11"),
        DFA.unpack(u"\1\12"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\13"),
        DFA.unpack(u"\1\14"),
        DFA.unpack(u"\1\15"),
        DFA.unpack(u"\1\16"),
        DFA.unpack(u"\1\17\21\uffff\1\12"),
        DFA.unpack(u"\1\10\56\uffff\2\11")
    ]

    # class definition for DFA #43

    DFA43 = DFA
 

    FOLLOW_grammarType_in_grammarDef52 = frozenset([2])
    FOLLOW_ID_in_grammarDef54 = frozenset([4, 7, 31, 41, 46, 72])
    FOLLOW_DOC_COMMENT_in_grammarDef56 = frozenset([4, 7, 31, 41, 46, 72])
    FOLLOW_optionsSpec_in_grammarDef59 = frozenset([4, 7, 31, 41, 46, 72])
    FOLLOW_tokensSpec_in_grammarDef62 = frozenset([4, 7, 31, 41, 46, 72])
    FOLLOW_attrScope_in_grammarDef65 = frozenset([4, 7, 31, 41, 46, 72])
    FOLLOW_action_in_grammarDef68 = frozenset([4, 7, 31, 41, 46, 72])
    FOLLOW_rule_in_grammarDef71 = frozenset([3, 4, 7, 31, 41, 46, 72])
    FOLLOW_set_in_grammarType0 = frozenset([1])
    FOLLOW_TOKENS_in_tokensSpec127 = frozenset([2])
    FOLLOW_tokenSpec_in_tokensSpec129 = frozenset([3, 42, 71])
    FOLLOW_71_in_tokenSpec143 = frozenset([2])
    FOLLOW_TOKEN_REF_in_tokenSpec145 = frozenset([43])
    FOLLOW_STRING_LITERAL_in_tokenSpec147 = frozenset([3])
    FOLLOW_71_in_tokenSpec154 = frozenset([2])
    FOLLOW_TOKEN_REF_in_tokenSpec156 = frozenset([44])
    FOLLOW_CHAR_LITERAL_in_tokenSpec158 = frozenset([3])
    FOLLOW_TOKEN_REF_in_tokenSpec164 = frozenset([1])
    FOLLOW_SCOPE_in_attrScope176 = frozenset([2])
    FOLLOW_ID_in_attrScope178 = frozenset([45])
    FOLLOW_ACTION_in_attrScope180 = frozenset([3])
    FOLLOW_72_in_action193 = frozenset([2])
    FOLLOW_ID_in_action195 = frozenset([20])
    FOLLOW_ID_in_action197 = frozenset([45])
    FOLLOW_ACTION_in_action199 = frozenset([3])
    FOLLOW_72_in_action206 = frozenset([2])
    FOLLOW_ID_in_action208 = frozenset([45])
    FOLLOW_ACTION_in_action210 = frozenset([3])
    FOLLOW_OPTIONS_in_optionsSpec223 = frozenset([2])
    FOLLOW_option_in_optionsSpec225 = frozenset([3, 71])
    FOLLOW_71_in_option244 = frozenset([2])
    FOLLOW_ID_in_option246 = frozenset([20, 43, 44, 47])
    FOLLOW_optionValue_in_option248 = frozenset([3])
    FOLLOW_set_in_optionValue0 = frozenset([1])
    FOLLOW_RULE_in_rule314 = frozenset([2])
    FOLLOW_ID_in_rule316 = frozenset([8, 21, 23, 31, 36, 46, 72, 75, 76, 77])
    FOLLOW_modifier_in_rule318 = frozenset([8, 21, 23, 31, 36, 46, 72, 75, 76, 77])
    FOLLOW_ARG_in_rule323 = frozenset([2])
    FOLLOW_ARG_ACTION_in_rule325 = frozenset([3])
    FOLLOW_RET_in_rule332 = frozenset([2])
    FOLLOW_ARG_ACTION_in_rule334 = frozenset([3])
    FOLLOW_optionsSpec_in_rule347 = frozenset([8, 21, 23, 31, 36, 46, 72, 75, 76, 77])
    FOLLOW_ruleScopeSpec_in_rule350 = frozenset([8, 21, 23, 31, 36, 46, 72, 75, 76, 77])
    FOLLOW_ruleAction_in_rule353 = frozenset([8, 21, 23, 31, 36, 46, 72, 75, 76, 77])
    FOLLOW_altList_in_rule364 = frozenset([17, 85, 86])
    FOLLOW_exceptionGroup_in_rule374 = frozenset([17])
    FOLLOW_EOR_in_rule377 = frozenset([3])
    FOLLOW_set_in_modifier0 = frozenset([1])
    FOLLOW_72_in_ruleAction416 = frozenset([2])
    FOLLOW_ID_in_ruleAction418 = frozenset([45])
    FOLLOW_ACTION_in_ruleAction420 = frozenset([3])
    FOLLOW_80_in_throwsSpec433 = frozenset([2])
    FOLLOW_ID_in_throwsSpec435 = frozenset([3, 20])
    FOLLOW_SCOPE_in_ruleScopeSpec449 = frozenset([2])
    FOLLOW_ACTION_in_ruleScopeSpec451 = frozenset([3])
    FOLLOW_SCOPE_in_ruleScopeSpec458 = frozenset([2])
    FOLLOW_ACTION_in_ruleScopeSpec460 = frozenset([20])
    FOLLOW_ID_in_ruleScopeSpec462 = frozenset([3, 20])
    FOLLOW_SCOPE_in_ruleScopeSpec470 = frozenset([2])
    FOLLOW_ID_in_ruleScopeSpec472 = frozenset([3, 20])
    FOLLOW_BLOCK_in_block492 = frozenset([2])
    FOLLOW_optionsSpec_in_block494 = frozenset([16])
    FOLLOW_alternative_in_block498 = frozenset([16, 18, 40])
    FOLLOW_rewrite_in_block500 = frozenset([16, 18])
    FOLLOW_EOB_in_block504 = frozenset([3])
    FOLLOW_BLOCK_in_altList527 = frozenset([2])
    FOLLOW_alternative_in_altList530 = frozenset([16, 18, 40])
    FOLLOW_rewrite_in_altList532 = frozenset([16, 18])
    FOLLOW_EOB_in_altList536 = frozenset([3])
    FOLLOW_ALT_in_alternative558 = frozenset([2])
    FOLLOW_element_in_alternative560 = frozenset([8, 9, 10, 11, 12, 14, 19, 32, 33, 34, 37, 38, 39, 42, 43, 44, 45, 49, 71, 87, 89, 92])
    FOLLOW_EOA_in_alternative563 = frozenset([3])
    FOLLOW_ALT_in_alternative575 = frozenset([2])
    FOLLOW_EPSILON_in_alternative577 = frozenset([19])
    FOLLOW_EOA_in_alternative579 = frozenset([3])
    FOLLOW_exceptionHandler_in_exceptionGroup594 = frozenset([1, 85, 86])
    FOLLOW_finallyClause_in_exceptionGroup597 = frozenset([1])
    FOLLOW_finallyClause_in_exceptionGroup603 = frozenset([1])
    FOLLOW_85_in_exceptionHandler624 = frozenset([2])
    FOLLOW_ARG_ACTION_in_exceptionHandler626 = frozenset([45])
    FOLLOW_ACTION_in_exceptionHandler628 = frozenset([3])
    FOLLOW_86_in_finallyClause650 = frozenset([2])
    FOLLOW_ACTION_in_finallyClause652 = frozenset([3])
    FOLLOW_elementNoOptionSpec_in_element667 = frozenset([1])
    FOLLOW_set_in_elementNoOptionSpec679 = frozenset([2])
    FOLLOW_ID_in_elementNoOptionSpec685 = frozenset([8, 9, 10, 11, 12, 34])
    FOLLOW_block_in_elementNoOptionSpec687 = frozenset([3])
    FOLLOW_set_in_elementNoOptionSpec694 = frozenset([2])
    FOLLOW_ID_in_elementNoOptionSpec700 = frozenset([14, 38, 39, 42, 43, 44, 49, 89, 92])
    FOLLOW_atom_in_elementNoOptionSpec702 = frozenset([3])
    FOLLOW_atom_in_elementNoOptionSpec708 = frozenset([1])
    FOLLOW_ebnf_in_elementNoOptionSpec713 = frozenset([1])
    FOLLOW_ACTION_in_elementNoOptionSpec720 = frozenset([1])
    FOLLOW_SEMPRED_in_elementNoOptionSpec727 = frozenset([1])
    FOLLOW_GATED_SEMPRED_in_elementNoOptionSpec732 = frozenset([1])
    FOLLOW_treeSpec_in_elementNoOptionSpec739 = frozenset([1])
    FOLLOW_set_in_atom751 = frozenset([2])
    FOLLOW_atom_in_atom757 = frozenset([3])
    FOLLOW_range_in_atom763 = frozenset([1])
    FOLLOW_notSet_in_atom768 = frozenset([1])
    FOLLOW_RULE_REF_in_atom777 = frozenset([2])
    FOLLOW_ARG_ACTION_in_atom779 = frozenset([3])
    FOLLOW_RULE_REF_in_atom788 = frozenset([1])
    FOLLOW_terminal_in_atom798 = frozenset([1])
    FOLLOW_89_in_notSet813 = frozenset([2])
    FOLLOW_notTerminal_in_notSet815 = frozenset([3])
    FOLLOW_89_in_notSet822 = frozenset([2])
    FOLLOW_block_in_notSet824 = frozenset([3])
    FOLLOW_TREE_BEGIN_in_treeSpec837 = frozenset([2])
    FOLLOW_element_in_treeSpec839 = frozenset([3, 8, 9, 10, 11, 12, 14, 19, 32, 33, 34, 37, 38, 39, 42, 43, 44, 45, 49, 71, 87, 89, 92])
    FOLLOW_SYNPRED_in_ebnf855 = frozenset([2])
    FOLLOW_block_in_ebnf857 = frozenset([3])
    FOLLOW_SYN_SEMPRED_in_ebnf863 = frozenset([1])
    FOLLOW_ebnfSuffix_in_ebnf869 = frozenset([2])
    FOLLOW_block_in_ebnf871 = frozenset([3])
    FOLLOW_block_in_ebnf877 = frozenset([1])
    FOLLOW_CHAR_RANGE_in_range889 = frozenset([2])
    FOLLOW_CHAR_LITERAL_in_range891 = frozenset([44])
    FOLLOW_CHAR_LITERAL_in_range893 = frozenset([3])
    FOLLOW_CHAR_LITERAL_in_terminal910 = frozenset([1])
    FOLLOW_TOKEN_REF_in_terminal918 = frozenset([1])
    FOLLOW_STRING_LITERAL_in_terminal926 = frozenset([1])
    FOLLOW_TOKEN_REF_in_terminal935 = frozenset([2])
    FOLLOW_ARG_ACTION_in_terminal937 = frozenset([3])
    FOLLOW_92_in_terminal946 = frozenset([1])
    FOLLOW_set_in_notTerminal0 = frozenset([1])
    FOLLOW_set_in_ebnfSuffix0 = frozenset([1])
    FOLLOW_REWRITE_in_rewrite1012 = frozenset([2])
    FOLLOW_SEMPRED_in_rewrite1014 = frozenset([16, 30, 45])
    FOLLOW_rewrite_alternative_in_rewrite1016 = frozenset([3])
    FOLLOW_REWRITE_in_rewrite1022 = frozenset([2])
    FOLLOW_rewrite_alternative_in_rewrite1024 = frozenset([3])
    FOLLOW_rewrite_template_in_rewrite_alternative1039 = frozenset([1])
    FOLLOW_rewrite_tree_alternative_in_rewrite_alternative1044 = frozenset([1])
    FOLLOW_ALT_in_rewrite_alternative1055 = frozenset([2])
    FOLLOW_EPSILON_in_rewrite_alternative1057 = frozenset([19])
    FOLLOW_EOA_in_rewrite_alternative1059 = frozenset([3])
    FOLLOW_BLOCK_in_rewrite_tree_block1078 = frozenset([2])
    FOLLOW_rewrite_tree_alternative_in_rewrite_tree_block1080 = frozenset([18])
    FOLLOW_EOB_in_rewrite_tree_block1082 = frozenset([3])
    FOLLOW_ALT_in_rewrite_tree_alternative1101 = frozenset([2])
    FOLLOW_rewrite_tree_element_in_rewrite_tree_alternative1103 = frozenset([8, 9, 10, 11, 19, 29, 37, 42, 43, 44, 45, 49])
    FOLLOW_EOA_in_rewrite_tree_alternative1106 = frozenset([3])
    FOLLOW_rewrite_tree_atom_in_rewrite_tree_element1121 = frozenset([1])
    FOLLOW_rewrite_tree_in_rewrite_tree_element1126 = frozenset([1])
    FOLLOW_rewrite_tree_block_in_rewrite_tree_element1133 = frozenset([1])
    FOLLOW_rewrite_tree_ebnf_in_rewrite_tree_element1140 = frozenset([1])
    FOLLOW_CHAR_LITERAL_in_rewrite_tree_atom1156 = frozenset([1])
    FOLLOW_TOKEN_REF_in_rewrite_tree_atom1163 = frozenset([1])
    FOLLOW_TOKEN_REF_in_rewrite_tree_atom1171 = frozenset([2])
    FOLLOW_ARG_ACTION_in_rewrite_tree_atom1173 = frozenset([3])
    FOLLOW_RULE_REF_in_rewrite_tree_atom1185 = frozenset([1])
    FOLLOW_STRING_LITERAL_in_rewrite_tree_atom1192 = frozenset([1])
    FOLLOW_LABEL_in_rewrite_tree_atom1199 = frozenset([1])
    FOLLOW_ACTION_in_rewrite_tree_atom1204 = frozenset([1])
    FOLLOW_ebnfSuffix_in_rewrite_tree_ebnf1216 = frozenset([2])
    FOLLOW_rewrite_tree_block_in_rewrite_tree_ebnf1218 = frozenset([3])
    FOLLOW_TREE_BEGIN_in_rewrite_tree1232 = frozenset([2])
    FOLLOW_rewrite_tree_atom_in_rewrite_tree1234 = frozenset([3, 8, 9, 10, 11, 19, 29, 37, 42, 43, 44, 45, 49])
    FOLLOW_rewrite_tree_element_in_rewrite_tree1236 = frozenset([3, 8, 9, 10, 11, 19, 29, 37, 42, 43, 44, 45, 49])
    FOLLOW_TEMPLATE_in_rewrite_template1254 = frozenset([2])
    FOLLOW_ID_in_rewrite_template1256 = frozenset([22])
    FOLLOW_rewrite_template_args_in_rewrite_template1258 = frozenset([50, 51])
    FOLLOW_set_in_rewrite_template1265 = frozenset([3])
    FOLLOW_rewrite_template_ref_in_rewrite_template1281 = frozenset([1])
    FOLLOW_rewrite_indirect_template_head_in_rewrite_template1286 = frozenset([1])
    FOLLOW_ACTION_in_rewrite_template1291 = frozenset([1])
    FOLLOW_TEMPLATE_in_rewrite_template_ref1305 = frozenset([2])
    FOLLOW_ID_in_rewrite_template_ref1307 = frozenset([22])
    FOLLOW_rewrite_template_args_in_rewrite_template_ref1309 = frozenset([3])
    FOLLOW_TEMPLATE_in_rewrite_indirect_template_head1324 = frozenset([2])
    FOLLOW_ACTION_in_rewrite_indirect_template_head1326 = frozenset([22])
    FOLLOW_rewrite_template_args_in_rewrite_indirect_template_head1328 = frozenset([3])
    FOLLOW_ARGLIST_in_rewrite_template_args1341 = frozenset([2])
    FOLLOW_rewrite_template_arg_in_rewrite_template_args1343 = frozenset([3, 21])
    FOLLOW_ARGLIST_in_rewrite_template_args1350 = frozenset([1])
    FOLLOW_ARG_in_rewrite_template_arg1364 = frozenset([2])
    FOLLOW_ID_in_rewrite_template_arg1366 = frozenset([45])
    FOLLOW_ACTION_in_rewrite_template_arg1368 = frozenset([3])



def main(argv, stdin=sys.stdin, stdout=sys.stdout, stderr=sys.stderr):
    from antlr3.main import WalkerMain
    main = WalkerMain(ANTLRv3Tree_py)
    main.stdin = stdin
    main.stdout = stdout
    main.stderr = stderr
    main.execute(argv)


if __name__ == '__main__':
    main(sys.argv)
