# $ANTLR 3.2 Sep 23, 2009 12:02:23 /home/res/ANTLRv3_py.g 2009-11-28 18:18:12

import sys
from antlr3 import *
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


class ANTLRv3_pyLexer(Lexer):

    grammarFileName = "/home/res/ANTLRv3_py.g"
    #antlr_version = version_str_to_tuple("3.2 Sep 23, 2009 12:02:23")
    #antlr_version_str = "3.2 Sep 23, 2009 12:02:23"
    antlr_version = version_str_to_tuple("3.1")
    antlr_version_str = "3.1"

    def __init__(self, input=None, state=None):
        if state is None:
            state = RecognizerSharedState()
        super(ANTLRv3_pyLexer, self).__init__(input, state)


        self.dfa2 = self.DFA2(
            self, 2,
            eot = self.DFA2_eot,
            eof = self.DFA2_eof,
            min = self.DFA2_min,
            max = self.DFA2_max,
            accept = self.DFA2_accept,
            special = self.DFA2_special,
            transition = self.DFA2_transition
            )

        self.dfa9 = self.DFA9(
            self, 9,
            eot = self.DFA9_eot,
            eof = self.DFA9_eof,
            min = self.DFA9_min,
            max = self.DFA9_max,
            accept = self.DFA9_accept,
            special = self.DFA9_special,
            transition = self.DFA9_transition
            )

        self.dfa13 = self.DFA13(
            self, 13,
            eot = self.DFA13_eot,
            eof = self.DFA13_eof,
            min = self.DFA13_min,
            max = self.DFA13_max,
            accept = self.DFA13_accept,
            special = self.DFA13_special,
            transition = self.DFA13_transition
            )

        self.dfa22 = self.DFA22(
            self, 22,
            eot = self.DFA22_eot,
            eof = self.DFA22_eof,
            min = self.DFA22_min,
            max = self.DFA22_max,
            accept = self.DFA22_accept,
            special = self.DFA22_special,
            transition = self.DFA22_transition
            )






    # $ANTLR start "SCOPE"
    def mSCOPE(self, ):

        try:
            _type = SCOPE
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:7:7: ( 'scope' )
            # /home/res/ANTLRv3_py.g:7:9: 'scope'
            pass 
            self.match("scope")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "SCOPE"



    # $ANTLR start "FRAGMENT"
    def mFRAGMENT(self, ):

        try:
            _type = FRAGMENT
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:8:10: ( 'fragment' )
            # /home/res/ANTLRv3_py.g:8:12: 'fragment'
            pass 
            self.match("fragment")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "FRAGMENT"



    # $ANTLR start "TREE_BEGIN"
    def mTREE_BEGIN(self, ):

        try:
            _type = TREE_BEGIN
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:9:12: ( '^(' )
            # /home/res/ANTLRv3_py.g:9:14: '^('
            pass 
            self.match("^(")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "TREE_BEGIN"



    # $ANTLR start "ROOT"
    def mROOT(self, ):

        try:
            _type = ROOT
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:10:6: ( '^' )
            # /home/res/ANTLRv3_py.g:10:8: '^'
            pass 
            self.match(94)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "ROOT"



    # $ANTLR start "BANG"
    def mBANG(self, ):

        try:
            _type = BANG
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:11:6: ( '!' )
            # /home/res/ANTLRv3_py.g:11:8: '!'
            pass 
            self.match(33)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "BANG"



    # $ANTLR start "RANGE"
    def mRANGE(self, ):

        try:
            _type = RANGE
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:12:7: ( '..' )
            # /home/res/ANTLRv3_py.g:12:9: '..'
            pass 
            self.match("..")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "RANGE"



    # $ANTLR start "REWRITE"
    def mREWRITE(self, ):

        try:
            _type = REWRITE
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:13:9: ( '->' )
            # /home/res/ANTLRv3_py.g:13:11: '->'
            pass 
            self.match("->")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "REWRITE"



    # $ANTLR start "T__65"
    def mT__65(self, ):

        try:
            _type = T__65
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:14:7: ( 'lexer' )
            # /home/res/ANTLRv3_py.g:14:9: 'lexer'
            pass 
            self.match("lexer")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__65"



    # $ANTLR start "T__66"
    def mT__66(self, ):

        try:
            _type = T__66
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:15:7: ( 'parser' )
            # /home/res/ANTLRv3_py.g:15:9: 'parser'
            pass 
            self.match("parser")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__66"



    # $ANTLR start "T__67"
    def mT__67(self, ):

        try:
            _type = T__67
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:16:7: ( 'tree' )
            # /home/res/ANTLRv3_py.g:16:9: 'tree'
            pass 
            self.match("tree")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__67"



    # $ANTLR start "T__68"
    def mT__68(self, ):

        try:
            _type = T__68
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:17:7: ( 'grammar' )
            # /home/res/ANTLRv3_py.g:17:9: 'grammar'
            pass 
            self.match("grammar")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__68"



    # $ANTLR start "T__69"
    def mT__69(self, ):

        try:
            _type = T__69
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:18:7: ( ';' )
            # /home/res/ANTLRv3_py.g:18:9: ';'
            pass 
            self.match(59)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__69"



    # $ANTLR start "T__70"
    def mT__70(self, ):

        try:
            _type = T__70
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:19:7: ( '}' )
            # /home/res/ANTLRv3_py.g:19:9: '}'
            pass 
            self.match(125)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__70"



    # $ANTLR start "T__71"
    def mT__71(self, ):

        try:
            _type = T__71
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:20:7: ( '=' )
            # /home/res/ANTLRv3_py.g:20:9: '='
            pass 
            self.match(61)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__71"



    # $ANTLR start "T__72"
    def mT__72(self, ):

        try:
            _type = T__72
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:21:7: ( '@' )
            # /home/res/ANTLRv3_py.g:21:9: '@'
            pass 
            self.match(64)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__72"



    # $ANTLR start "T__73"
    def mT__73(self, ):

        try:
            _type = T__73
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:22:7: ( '::' )
            # /home/res/ANTLRv3_py.g:22:9: '::'
            pass 
            self.match("::")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__73"



    # $ANTLR start "T__74"
    def mT__74(self, ):

        try:
            _type = T__74
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:23:7: ( '*' )
            # /home/res/ANTLRv3_py.g:23:9: '*'
            pass 
            self.match(42)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__74"



    # $ANTLR start "T__75"
    def mT__75(self, ):

        try:
            _type = T__75
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:24:7: ( 'protected' )
            # /home/res/ANTLRv3_py.g:24:9: 'protected'
            pass 
            self.match("protected")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__75"



    # $ANTLR start "T__76"
    def mT__76(self, ):

        try:
            _type = T__76
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:25:7: ( 'public' )
            # /home/res/ANTLRv3_py.g:25:9: 'public'
            pass 
            self.match("public")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__76"



    # $ANTLR start "T__77"
    def mT__77(self, ):

        try:
            _type = T__77
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:26:7: ( 'private' )
            # /home/res/ANTLRv3_py.g:26:9: 'private'
            pass 
            self.match("private")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__77"



    # $ANTLR start "T__78"
    def mT__78(self, ):

        try:
            _type = T__78
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:27:7: ( 'returns' )
            # /home/res/ANTLRv3_py.g:27:9: 'returns'
            pass 
            self.match("returns")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__78"



    # $ANTLR start "T__79"
    def mT__79(self, ):

        try:
            _type = T__79
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:28:7: ( ':' )
            # /home/res/ANTLRv3_py.g:28:9: ':'
            pass 
            self.match(58)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__79"



    # $ANTLR start "T__80"
    def mT__80(self, ):

        try:
            _type = T__80
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:29:7: ( 'throws' )
            # /home/res/ANTLRv3_py.g:29:9: 'throws'
            pass 
            self.match("throws")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__80"



    # $ANTLR start "T__81"
    def mT__81(self, ):

        try:
            _type = T__81
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:30:7: ( ',' )
            # /home/res/ANTLRv3_py.g:30:9: ','
            pass 
            self.match(44)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__81"



    # $ANTLR start "T__82"
    def mT__82(self, ):

        try:
            _type = T__82
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:31:7: ( '(' )
            # /home/res/ANTLRv3_py.g:31:9: '('
            pass 
            self.match(40)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__82"



    # $ANTLR start "T__83"
    def mT__83(self, ):

        try:
            _type = T__83
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:32:7: ( '|' )
            # /home/res/ANTLRv3_py.g:32:9: '|'
            pass 
            self.match(124)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__83"



    # $ANTLR start "T__84"
    def mT__84(self, ):

        try:
            _type = T__84
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:33:7: ( ')' )
            # /home/res/ANTLRv3_py.g:33:9: ')'
            pass 
            self.match(41)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__84"



    # $ANTLR start "T__85"
    def mT__85(self, ):

        try:
            _type = T__85
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:34:7: ( 'catch' )
            # /home/res/ANTLRv3_py.g:34:9: 'catch'
            pass 
            self.match("catch")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__85"



    # $ANTLR start "T__86"
    def mT__86(self, ):

        try:
            _type = T__86
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:35:7: ( 'finally' )
            # /home/res/ANTLRv3_py.g:35:9: 'finally'
            pass 
            self.match("finally")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__86"



    # $ANTLR start "T__87"
    def mT__87(self, ):

        try:
            _type = T__87
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:36:7: ( '+=' )
            # /home/res/ANTLRv3_py.g:36:9: '+='
            pass 
            self.match("+=")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__87"



    # $ANTLR start "T__88"
    def mT__88(self, ):

        try:
            _type = T__88
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:37:7: ( '=>' )
            # /home/res/ANTLRv3_py.g:37:9: '=>'
            pass 
            self.match("=>")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__88"



    # $ANTLR start "T__89"
    def mT__89(self, ):

        try:
            _type = T__89
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:38:7: ( '~' )
            # /home/res/ANTLRv3_py.g:38:9: '~'
            pass 
            self.match(126)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__89"



    # $ANTLR start "T__90"
    def mT__90(self, ):

        try:
            _type = T__90
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:39:7: ( '?' )
            # /home/res/ANTLRv3_py.g:39:9: '?'
            pass 
            self.match(63)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__90"



    # $ANTLR start "T__91"
    def mT__91(self, ):

        try:
            _type = T__91
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:40:7: ( '+' )
            # /home/res/ANTLRv3_py.g:40:9: '+'
            pass 
            self.match(43)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__91"



    # $ANTLR start "T__92"
    def mT__92(self, ):

        try:
            _type = T__92
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:41:7: ( '.' )
            # /home/res/ANTLRv3_py.g:41:9: '.'
            pass 
            self.match(46)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__92"



    # $ANTLR start "T__93"
    def mT__93(self, ):

        try:
            _type = T__93
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:42:7: ( '$' )
            # /home/res/ANTLRv3_py.g:42:9: '$'
            pass 
            self.match(36)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "T__93"



    # $ANTLR start "SL_COMMENT"
    def mSL_COMMENT(self, ):

        try:
            _type = SL_COMMENT
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:463:3: ( '//' ( ' $ANTLR ' SRC | (~ ( '\\r' | '\\n' ) )* ) ( '\\r' )? '\\n' )
            # /home/res/ANTLRv3_py.g:463:5: '//' ( ' $ANTLR ' SRC | (~ ( '\\r' | '\\n' ) )* ) ( '\\r' )? '\\n'
            pass 
            self.match("//")
            # /home/res/ANTLRv3_py.g:464:5: ( ' $ANTLR ' SRC | (~ ( '\\r' | '\\n' ) )* )
            alt2 = 2
            alt2 = self.dfa2.predict(self.input)
            if alt2 == 1:
                # /home/res/ANTLRv3_py.g:464:7: ' $ANTLR ' SRC
                pass 
                self.match(" $ANTLR ")
                self.mSRC()


            elif alt2 == 2:
                # /home/res/ANTLRv3_py.g:465:6: (~ ( '\\r' | '\\n' ) )*
                pass 
                # /home/res/ANTLRv3_py.g:465:6: (~ ( '\\r' | '\\n' ) )*
                while True: #loop1
                    alt1 = 2
                    LA1_0 = self.input.LA(1)

                    if ((0 <= LA1_0 <= 9) or (11 <= LA1_0 <= 12) or (14 <= LA1_0 <= 65535)) :
                        alt1 = 1


                    if alt1 == 1:
                        # /home/res/ANTLRv3_py.g:465:6: ~ ( '\\r' | '\\n' )
                        pass 
                        if (0 <= self.input.LA(1) <= 9) or (11 <= self.input.LA(1) <= 12) or (14 <= self.input.LA(1) <= 65535):
                            self.input.consume()
                        else:
                            mse = MismatchedSetException(None, self.input)
                            self.recover(mse)
                            raise mse



                    else:
                        break #loop1



            # /home/res/ANTLRv3_py.g:467:3: ( '\\r' )?
            alt3 = 2
            LA3_0 = self.input.LA(1)

            if (LA3_0 == 13) :
                alt3 = 1
            if alt3 == 1:
                # /home/res/ANTLRv3_py.g:467:3: '\\r'
                pass 
                self.match(13)



            self.match(10)
            #action start
            _channel=HIDDEN;
            #action end



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "SL_COMMENT"



    # $ANTLR start "ML_COMMENT"
    def mML_COMMENT(self, ):

        try:
            _type = ML_COMMENT
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:472:2: ( '/*' ( . )* '*/' )
            # /home/res/ANTLRv3_py.g:472:4: '/*' ( . )* '*/'
            pass 
            self.match("/*")
            #action start
             
            if (self.input.LA(1)==42):
            	_type=DOC_COMMENT
            else:
            	_channel=HIDDEN

            #action end
            # /home/res/ANTLRv3_py.g:478:3: ( . )*
            while True: #loop4
                alt4 = 2
                LA4_0 = self.input.LA(1)

                if (LA4_0 == 42) :
                    LA4_1 = self.input.LA(2)

                    if (LA4_1 == 47) :
                        alt4 = 2
                    elif ((0 <= LA4_1 <= 46) or (48 <= LA4_1 <= 65535)) :
                        alt4 = 1


                elif ((0 <= LA4_0 <= 41) or (43 <= LA4_0 <= 65535)) :
                    alt4 = 1


                if alt4 == 1:
                    # /home/res/ANTLRv3_py.g:478:3: .
                    pass 
                    self.matchAny()


                else:
                    break #loop4
            self.match("*/")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "ML_COMMENT"



    # $ANTLR start "CHAR_LITERAL"
    def mCHAR_LITERAL(self, ):

        try:
            _type = CHAR_LITERAL
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:482:2: ( '\\'' LITERAL_CHAR '\\'' )
            # /home/res/ANTLRv3_py.g:482:4: '\\'' LITERAL_CHAR '\\''
            pass 
            self.match(39)
            self.mLITERAL_CHAR()
            self.match(39)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "CHAR_LITERAL"



    # $ANTLR start "STRING_LITERAL"
    def mSTRING_LITERAL(self, ):

        try:
            _type = STRING_LITERAL
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:486:2: ( '\\'' LITERAL_CHAR ( LITERAL_CHAR )* '\\'' )
            # /home/res/ANTLRv3_py.g:486:4: '\\'' LITERAL_CHAR ( LITERAL_CHAR )* '\\''
            pass 
            self.match(39)
            self.mLITERAL_CHAR()
            # /home/res/ANTLRv3_py.g:486:22: ( LITERAL_CHAR )*
            while True: #loop5
                alt5 = 2
                LA5_0 = self.input.LA(1)

                if ((0 <= LA5_0 <= 38) or (40 <= LA5_0 <= 65535)) :
                    alt5 = 1


                if alt5 == 1:
                    # /home/res/ANTLRv3_py.g:486:22: LITERAL_CHAR
                    pass 
                    self.mLITERAL_CHAR()


                else:
                    break #loop5
            self.match(39)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "STRING_LITERAL"



    # $ANTLR start "LITERAL_CHAR"
    def mLITERAL_CHAR(self, ):

        try:
            # /home/res/ANTLRv3_py.g:491:2: ( ESC | ~ ( '\\'' | '\\\\' ) )
            alt6 = 2
            LA6_0 = self.input.LA(1)

            if (LA6_0 == 92) :
                alt6 = 1
            elif ((0 <= LA6_0 <= 38) or (40 <= LA6_0 <= 91) or (93 <= LA6_0 <= 65535)) :
                alt6 = 2
            else:
                nvae = NoViableAltException("", 6, 0, self.input)

                raise nvae

            if alt6 == 1:
                # /home/res/ANTLRv3_py.g:491:4: ESC
                pass 
                self.mESC()


            elif alt6 == 2:
                # /home/res/ANTLRv3_py.g:492:4: ~ ( '\\'' | '\\\\' )
                pass 
                if (0 <= self.input.LA(1) <= 38) or (40 <= self.input.LA(1) <= 91) or (93 <= self.input.LA(1) <= 65535):
                    self.input.consume()
                else:
                    mse = MismatchedSetException(None, self.input)
                    self.recover(mse)
                    raise mse




        finally:

            pass

    # $ANTLR end "LITERAL_CHAR"



    # $ANTLR start "DOUBLE_QUOTE_STRING_LITERAL"
    def mDOUBLE_QUOTE_STRING_LITERAL(self, ):

        try:
            _type = DOUBLE_QUOTE_STRING_LITERAL
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:496:2: ( '\"' ( ESC | ~ ( '\\\\' | '\"' ) )* '\"' )
            # /home/res/ANTLRv3_py.g:496:4: '\"' ( ESC | ~ ( '\\\\' | '\"' ) )* '\"'
            pass 
            self.match(34)
            # /home/res/ANTLRv3_py.g:496:8: ( ESC | ~ ( '\\\\' | '\"' ) )*
            while True: #loop7
                alt7 = 3
                LA7_0 = self.input.LA(1)

                if (LA7_0 == 92) :
                    alt7 = 1
                elif ((0 <= LA7_0 <= 33) or (35 <= LA7_0 <= 91) or (93 <= LA7_0 <= 65535)) :
                    alt7 = 2


                if alt7 == 1:
                    # /home/res/ANTLRv3_py.g:496:9: ESC
                    pass 
                    self.mESC()


                elif alt7 == 2:
                    # /home/res/ANTLRv3_py.g:496:15: ~ ( '\\\\' | '\"' )
                    pass 
                    if (0 <= self.input.LA(1) <= 33) or (35 <= self.input.LA(1) <= 91) or (93 <= self.input.LA(1) <= 65535):
                        self.input.consume()
                    else:
                        mse = MismatchedSetException(None, self.input)
                        self.recover(mse)
                        raise mse



                else:
                    break #loop7
            self.match(34)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "DOUBLE_QUOTE_STRING_LITERAL"



    # $ANTLR start "DOUBLE_ANGLE_STRING_LITERAL"
    def mDOUBLE_ANGLE_STRING_LITERAL(self, ):

        try:
            _type = DOUBLE_ANGLE_STRING_LITERAL
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:500:2: ( '<<' ( . )* '>>' )
            # /home/res/ANTLRv3_py.g:500:4: '<<' ( . )* '>>'
            pass 
            self.match("<<")
            # /home/res/ANTLRv3_py.g:500:9: ( . )*
            while True: #loop8
                alt8 = 2
                LA8_0 = self.input.LA(1)

                if (LA8_0 == 62) :
                    LA8_1 = self.input.LA(2)

                    if (LA8_1 == 62) :
                        alt8 = 2
                    elif ((0 <= LA8_1 <= 61) or (63 <= LA8_1 <= 65535)) :
                        alt8 = 1


                elif ((0 <= LA8_0 <= 61) or (63 <= LA8_0 <= 65535)) :
                    alt8 = 1


                if alt8 == 1:
                    # /home/res/ANTLRv3_py.g:500:9: .
                    pass 
                    self.matchAny()


                else:
                    break #loop8
            self.match(">>")



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "DOUBLE_ANGLE_STRING_LITERAL"



    # $ANTLR start "ESC"
    def mESC(self, ):

        try:
            # /home/res/ANTLRv3_py.g:504:5: ( '\\\\' ( 'n' | 'r' | 't' | 'b' | 'f' | '\"' | '\\'' | '\\\\' | '>' | 'u' XDIGIT XDIGIT XDIGIT XDIGIT | . ) )
            # /home/res/ANTLRv3_py.g:504:7: '\\\\' ( 'n' | 'r' | 't' | 'b' | 'f' | '\"' | '\\'' | '\\\\' | '>' | 'u' XDIGIT XDIGIT XDIGIT XDIGIT | . )
            pass 
            self.match(92)
            # /home/res/ANTLRv3_py.g:505:3: ( 'n' | 'r' | 't' | 'b' | 'f' | '\"' | '\\'' | '\\\\' | '>' | 'u' XDIGIT XDIGIT XDIGIT XDIGIT | . )
            alt9 = 11
            alt9 = self.dfa9.predict(self.input)
            if alt9 == 1:
                # /home/res/ANTLRv3_py.g:505:5: 'n'
                pass 
                self.match(110)


            elif alt9 == 2:
                # /home/res/ANTLRv3_py.g:506:5: 'r'
                pass 
                self.match(114)


            elif alt9 == 3:
                # /home/res/ANTLRv3_py.g:507:5: 't'
                pass 
                self.match(116)


            elif alt9 == 4:
                # /home/res/ANTLRv3_py.g:508:5: 'b'
                pass 
                self.match(98)


            elif alt9 == 5:
                # /home/res/ANTLRv3_py.g:509:5: 'f'
                pass 
                self.match(102)


            elif alt9 == 6:
                # /home/res/ANTLRv3_py.g:510:5: '\"'
                pass 
                self.match(34)


            elif alt9 == 7:
                # /home/res/ANTLRv3_py.g:511:5: '\\''
                pass 
                self.match(39)


            elif alt9 == 8:
                # /home/res/ANTLRv3_py.g:512:5: '\\\\'
                pass 
                self.match(92)


            elif alt9 == 9:
                # /home/res/ANTLRv3_py.g:513:5: '>'
                pass 
                self.match(62)


            elif alt9 == 10:
                # /home/res/ANTLRv3_py.g:514:5: 'u' XDIGIT XDIGIT XDIGIT XDIGIT
                pass 
                self.match(117)
                self.mXDIGIT()
                self.mXDIGIT()
                self.mXDIGIT()
                self.mXDIGIT()


            elif alt9 == 11:
                # /home/res/ANTLRv3_py.g:515:5: .
                pass 
                self.matchAny()







        finally:

            pass

    # $ANTLR end "ESC"



    # $ANTLR start "XDIGIT"
    def mXDIGIT(self, ):

        try:
            # /home/res/ANTLRv3_py.g:520:8: ( '0' .. '9' | 'a' .. 'f' | 'A' .. 'F' )
            # /home/res/ANTLRv3_py.g:
            pass 
            if (48 <= self.input.LA(1) <= 57) or (65 <= self.input.LA(1) <= 70) or (97 <= self.input.LA(1) <= 102):
                self.input.consume()
            else:
                mse = MismatchedSetException(None, self.input)
                self.recover(mse)
                raise mse





        finally:

            pass

    # $ANTLR end "XDIGIT"



    # $ANTLR start "INT"
    def mINT(self, ):

        try:
            _type = INT
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:526:5: ( ( '0' .. '9' )+ )
            # /home/res/ANTLRv3_py.g:526:7: ( '0' .. '9' )+
            pass 
            # /home/res/ANTLRv3_py.g:526:7: ( '0' .. '9' )+
            cnt10 = 0
            while True: #loop10
                alt10 = 2
                LA10_0 = self.input.LA(1)

                if ((48 <= LA10_0 <= 57)) :
                    alt10 = 1


                if alt10 == 1:
                    # /home/res/ANTLRv3_py.g:526:7: '0' .. '9'
                    pass 
                    self.matchRange(48, 57)


                else:
                    if cnt10 >= 1:
                        break #loop10

                    eee = EarlyExitException(10, self.input)
                    raise eee

                cnt10 += 1



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "INT"



    # $ANTLR start "ARG_ACTION"
    def mARG_ACTION(self, ):

        try:
            _type = ARG_ACTION
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:530:2: ( NESTED_ARG_ACTION )
            # /home/res/ANTLRv3_py.g:530:4: NESTED_ARG_ACTION
            pass 
            self.mNESTED_ARG_ACTION()



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "ARG_ACTION"



    # $ANTLR start "NESTED_ARG_ACTION"
    def mNESTED_ARG_ACTION(self, ):

        try:
            # /home/res/ANTLRv3_py.g:534:19: ( '[' ( options {greedy=false; k=1; } : NESTED_ARG_ACTION | ACTION_STRING_LITERAL | ACTION_CHAR_LITERAL | . )* ']' )
            # /home/res/ANTLRv3_py.g:535:2: '[' ( options {greedy=false; k=1; } : NESTED_ARG_ACTION | ACTION_STRING_LITERAL | ACTION_CHAR_LITERAL | . )* ']'
            pass 
            self.match(91)
            # /home/res/ANTLRv3_py.g:536:2: ( options {greedy=false; k=1; } : NESTED_ARG_ACTION | ACTION_STRING_LITERAL | ACTION_CHAR_LITERAL | . )*
            while True: #loop11
                alt11 = 5
                LA11_0 = self.input.LA(1)

                if (LA11_0 == 93) :
                    alt11 = 5
                elif (LA11_0 == 91) :
                    alt11 = 1
                elif (LA11_0 == 34) :
                    alt11 = 2
                elif (LA11_0 == 39) :
                    alt11 = 3
                elif ((0 <= LA11_0 <= 33) or (35 <= LA11_0 <= 38) or (40 <= LA11_0 <= 90) or LA11_0 == 92 or (94 <= LA11_0 <= 65535)) :
                    alt11 = 4


                if alt11 == 1:
                    # /home/res/ANTLRv3_py.g:537:4: NESTED_ARG_ACTION
                    pass 
                    self.mNESTED_ARG_ACTION()


                elif alt11 == 2:
                    # /home/res/ANTLRv3_py.g:538:4: ACTION_STRING_LITERAL
                    pass 
                    self.mACTION_STRING_LITERAL()


                elif alt11 == 3:
                    # /home/res/ANTLRv3_py.g:539:4: ACTION_CHAR_LITERAL
                    pass 
                    self.mACTION_CHAR_LITERAL()


                elif alt11 == 4:
                    # /home/res/ANTLRv3_py.g:540:4: .
                    pass 
                    self.matchAny()


                else:
                    break #loop11
            self.match(93)
            #action start
            setText(getText().substring(1, getText().length()-1));
            #action end




        finally:

            pass

    # $ANTLR end "NESTED_ARG_ACTION"



    # $ANTLR start "ACTION"
    def mACTION(self, ):

        try:
            _type = ACTION
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:547:2: ( NESTED_ACTION ( '?' )? )
            # /home/res/ANTLRv3_py.g:547:4: NESTED_ACTION ( '?' )?
            pass 
            self.mNESTED_ACTION()
            # /home/res/ANTLRv3_py.g:547:18: ( '?' )?
            alt12 = 2
            LA12_0 = self.input.LA(1)

            if (LA12_0 == 63) :
                alt12 = 1
            if alt12 == 1:
                # /home/res/ANTLRv3_py.g:547:20: '?'
                pass 
                self.match(63)
                #action start
                _type = SEMPRED;
                #action end






            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "ACTION"



    # $ANTLR start "NESTED_ACTION"
    def mNESTED_ACTION(self, ):

        try:
            # /home/res/ANTLRv3_py.g:551:15: ( '{' ( options {greedy=false; k=2; } : NESTED_ACTION | SL_COMMENT | ML_COMMENT | ACTION_STRING_LITERAL | ACTION_CHAR_LITERAL | . )* '}' )
            # /home/res/ANTLRv3_py.g:552:2: '{' ( options {greedy=false; k=2; } : NESTED_ACTION | SL_COMMENT | ML_COMMENT | ACTION_STRING_LITERAL | ACTION_CHAR_LITERAL | . )* '}'
            pass 
            self.match(123)
            # /home/res/ANTLRv3_py.g:553:2: ( options {greedy=false; k=2; } : NESTED_ACTION | SL_COMMENT | ML_COMMENT | ACTION_STRING_LITERAL | ACTION_CHAR_LITERAL | . )*
            while True: #loop13
                alt13 = 7
                alt13 = self.dfa13.predict(self.input)
                if alt13 == 1:
                    # /home/res/ANTLRv3_py.g:554:4: NESTED_ACTION
                    pass 
                    self.mNESTED_ACTION()


                elif alt13 == 2:
                    # /home/res/ANTLRv3_py.g:555:4: SL_COMMENT
                    pass 
                    self.mSL_COMMENT()


                elif alt13 == 3:
                    # /home/res/ANTLRv3_py.g:556:4: ML_COMMENT
                    pass 
                    self.mML_COMMENT()


                elif alt13 == 4:
                    # /home/res/ANTLRv3_py.g:557:4: ACTION_STRING_LITERAL
                    pass 
                    self.mACTION_STRING_LITERAL()


                elif alt13 == 5:
                    # /home/res/ANTLRv3_py.g:558:4: ACTION_CHAR_LITERAL
                    pass 
                    self.mACTION_CHAR_LITERAL()


                elif alt13 == 6:
                    # /home/res/ANTLRv3_py.g:559:4: .
                    pass 
                    self.matchAny()


                else:
                    break #loop13
            self.match(125)




        finally:

            pass

    # $ANTLR end "NESTED_ACTION"



    # $ANTLR start "ACTION_CHAR_LITERAL"
    def mACTION_CHAR_LITERAL(self, ):

        try:
            # /home/res/ANTLRv3_py.g:566:2: ( '\\'' ( ACTION_ESC | ~ ( '\\\\' | '\\'' ) ) '\\'' )
            # /home/res/ANTLRv3_py.g:566:4: '\\'' ( ACTION_ESC | ~ ( '\\\\' | '\\'' ) ) '\\''
            pass 
            self.match(39)
            # /home/res/ANTLRv3_py.g:566:9: ( ACTION_ESC | ~ ( '\\\\' | '\\'' ) )
            alt14 = 2
            LA14_0 = self.input.LA(1)

            if (LA14_0 == 92) :
                alt14 = 1
            elif ((0 <= LA14_0 <= 38) or (40 <= LA14_0 <= 91) or (93 <= LA14_0 <= 65535)) :
                alt14 = 2
            else:
                nvae = NoViableAltException("", 14, 0, self.input)

                raise nvae

            if alt14 == 1:
                # /home/res/ANTLRv3_py.g:566:10: ACTION_ESC
                pass 
                self.mACTION_ESC()


            elif alt14 == 2:
                # /home/res/ANTLRv3_py.g:566:21: ~ ( '\\\\' | '\\'' )
                pass 
                if (0 <= self.input.LA(1) <= 38) or (40 <= self.input.LA(1) <= 91) or (93 <= self.input.LA(1) <= 65535):
                    self.input.consume()
                else:
                    mse = MismatchedSetException(None, self.input)
                    self.recover(mse)
                    raise mse




            self.match(39)




        finally:

            pass

    # $ANTLR end "ACTION_CHAR_LITERAL"



    # $ANTLR start "ACTION_STRING_LITERAL"
    def mACTION_STRING_LITERAL(self, ):

        try:
            # /home/res/ANTLRv3_py.g:571:2: ( '\"' ( ACTION_ESC | ~ ( '\\\\' | '\"' ) )* '\"' )
            # /home/res/ANTLRv3_py.g:571:4: '\"' ( ACTION_ESC | ~ ( '\\\\' | '\"' ) )* '\"'
            pass 
            self.match(34)
            # /home/res/ANTLRv3_py.g:571:8: ( ACTION_ESC | ~ ( '\\\\' | '\"' ) )*
            while True: #loop15
                alt15 = 3
                LA15_0 = self.input.LA(1)

                if (LA15_0 == 92) :
                    alt15 = 1
                elif ((0 <= LA15_0 <= 33) or (35 <= LA15_0 <= 91) or (93 <= LA15_0 <= 65535)) :
                    alt15 = 2


                if alt15 == 1:
                    # /home/res/ANTLRv3_py.g:571:9: ACTION_ESC
                    pass 
                    self.mACTION_ESC()


                elif alt15 == 2:
                    # /home/res/ANTLRv3_py.g:571:20: ~ ( '\\\\' | '\"' )
                    pass 
                    if (0 <= self.input.LA(1) <= 33) or (35 <= self.input.LA(1) <= 91) or (93 <= self.input.LA(1) <= 65535):
                        self.input.consume()
                    else:
                        mse = MismatchedSetException(None, self.input)
                        self.recover(mse)
                        raise mse



                else:
                    break #loop15
            self.match(34)




        finally:

            pass

    # $ANTLR end "ACTION_STRING_LITERAL"



    # $ANTLR start "ACTION_ESC"
    def mACTION_ESC(self, ):

        try:
            # /home/res/ANTLRv3_py.g:576:2: ( '\\\\\\'' | '\\\\' '\"' | '\\\\' ~ ( '\\'' | '\"' ) )
            alt16 = 3
            LA16_0 = self.input.LA(1)

            if (LA16_0 == 92) :
                LA16_1 = self.input.LA(2)

                if (LA16_1 == 39) :
                    alt16 = 1
                elif (LA16_1 == 34) :
                    alt16 = 2
                elif ((0 <= LA16_1 <= 33) or (35 <= LA16_1 <= 38) or (40 <= LA16_1 <= 65535)) :
                    alt16 = 3
                else:
                    nvae = NoViableAltException("", 16, 1, self.input)

                    raise nvae

            else:
                nvae = NoViableAltException("", 16, 0, self.input)

                raise nvae

            if alt16 == 1:
                # /home/res/ANTLRv3_py.g:576:4: '\\\\\\''
                pass 
                self.match("\\'")


            elif alt16 == 2:
                # /home/res/ANTLRv3_py.g:577:4: '\\\\' '\"'
                pass 
                self.match(92)
                self.match(34)


            elif alt16 == 3:
                # /home/res/ANTLRv3_py.g:578:4: '\\\\' ~ ( '\\'' | '\"' )
                pass 
                self.match(92)
                if (0 <= self.input.LA(1) <= 33) or (35 <= self.input.LA(1) <= 38) or (40 <= self.input.LA(1) <= 65535):
                    self.input.consume()
                else:
                    mse = MismatchedSetException(None, self.input)
                    self.recover(mse)
                    raise mse




        finally:

            pass

    # $ANTLR end "ACTION_ESC"



    # $ANTLR start "TOKEN_REF"
    def mTOKEN_REF(self, ):

        try:
            _type = TOKEN_REF
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:582:2: ( 'A' .. 'Z' ( 'a' .. 'z' | 'A' .. 'Z' | '_' | '0' .. '9' )* )
            # /home/res/ANTLRv3_py.g:582:4: 'A' .. 'Z' ( 'a' .. 'z' | 'A' .. 'Z' | '_' | '0' .. '9' )*
            pass 
            self.matchRange(65, 90)
            # /home/res/ANTLRv3_py.g:582:13: ( 'a' .. 'z' | 'A' .. 'Z' | '_' | '0' .. '9' )*
            while True: #loop17
                alt17 = 2
                LA17_0 = self.input.LA(1)

                if ((48 <= LA17_0 <= 57) or (65 <= LA17_0 <= 90) or LA17_0 == 95 or (97 <= LA17_0 <= 122)) :
                    alt17 = 1


                if alt17 == 1:
                    # /home/res/ANTLRv3_py.g:
                    pass 
                    if (48 <= self.input.LA(1) <= 57) or (65 <= self.input.LA(1) <= 90) or self.input.LA(1) == 95 or (97 <= self.input.LA(1) <= 122):
                        self.input.consume()
                    else:
                        mse = MismatchedSetException(None, self.input)
                        self.recover(mse)
                        raise mse



                else:
                    break #loop17



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "TOKEN_REF"



    # $ANTLR start "RULE_REF"
    def mRULE_REF(self, ):

        try:
            _type = RULE_REF
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:586:2: ( 'a' .. 'z' ( 'a' .. 'z' | 'A' .. 'Z' | '_' | '0' .. '9' )* )
            # /home/res/ANTLRv3_py.g:586:4: 'a' .. 'z' ( 'a' .. 'z' | 'A' .. 'Z' | '_' | '0' .. '9' )*
            pass 
            self.matchRange(97, 122)
            # /home/res/ANTLRv3_py.g:586:13: ( 'a' .. 'z' | 'A' .. 'Z' | '_' | '0' .. '9' )*
            while True: #loop18
                alt18 = 2
                LA18_0 = self.input.LA(1)

                if ((48 <= LA18_0 <= 57) or (65 <= LA18_0 <= 90) or LA18_0 == 95 or (97 <= LA18_0 <= 122)) :
                    alt18 = 1


                if alt18 == 1:
                    # /home/res/ANTLRv3_py.g:
                    pass 
                    if (48 <= self.input.LA(1) <= 57) or (65 <= self.input.LA(1) <= 90) or self.input.LA(1) == 95 or (97 <= self.input.LA(1) <= 122):
                        self.input.consume()
                    else:
                        mse = MismatchedSetException(None, self.input)
                        self.recover(mse)
                        raise mse



                else:
                    break #loop18



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "RULE_REF"



    # $ANTLR start "OPTIONS"
    def mOPTIONS(self, ):

        try:
            _type = OPTIONS
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:593:2: ( 'options' WS_LOOP '{' )
            # /home/res/ANTLRv3_py.g:593:4: 'options' WS_LOOP '{'
            pass 
            self.match("options")
            self.mWS_LOOP()
            self.match(123)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "OPTIONS"



    # $ANTLR start "TOKENS"
    def mTOKENS(self, ):

        try:
            _type = TOKENS
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:597:2: ( 'tokens' WS_LOOP '{' )
            # /home/res/ANTLRv3_py.g:597:4: 'tokens' WS_LOOP '{'
            pass 
            self.match("tokens")
            self.mWS_LOOP()
            self.match(123)



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "TOKENS"



    # $ANTLR start "SRC"
    def mSRC(self, ):

        try:
            file = None
            line = None

            # /home/res/ANTLRv3_py.g:605:5: ( 'src' ' ' file= ACTION_STRING_LITERAL ' ' line= INT )
            # /home/res/ANTLRv3_py.g:605:7: 'src' ' ' file= ACTION_STRING_LITERAL ' ' line= INT
            pass 
            self.match("src")
            self.match(32)
            fileStart989 = self.getCharIndex()
            self.mACTION_STRING_LITERAL()
            file = CommonToken(
                input=self.input, 
                type=INVALID_TOKEN_TYPE,
                channel=DEFAULT_CHANNEL,
                start=fileStart989,
                stop=self.getCharIndex()-1
                )
            self.match(32)
            lineStart995 = self.getCharIndex()
            self.mINT()
            line = CommonToken(
                input=self.input, 
                type=INVALID_TOKEN_TYPE,
                channel=DEFAULT_CHANNEL,
                start=lineStart995,
                stop=self.getCharIndex()-1
                )




        finally:

            pass

    # $ANTLR end "SRC"



    # $ANTLR start "WS"
    def mWS(self, ):

        try:
            _type = WS
            _channel = DEFAULT_CHANNEL

            # /home/res/ANTLRv3_py.g:608:4: ( ( ' ' | '\\t' | ( '\\r' )? '\\n' )+ )
            # /home/res/ANTLRv3_py.g:608:6: ( ' ' | '\\t' | ( '\\r' )? '\\n' )+
            pass 
            # /home/res/ANTLRv3_py.g:608:6: ( ' ' | '\\t' | ( '\\r' )? '\\n' )+
            cnt20 = 0
            while True: #loop20
                alt20 = 4
                LA20 = self.input.LA(1)
                if LA20 == 32:
                    alt20 = 1
                elif LA20 == 9:
                    alt20 = 2
                elif LA20 == 10 or LA20 == 13:
                    alt20 = 3

                if alt20 == 1:
                    # /home/res/ANTLRv3_py.g:608:8: ' '
                    pass 
                    self.match(32)


                elif alt20 == 2:
                    # /home/res/ANTLRv3_py.g:609:5: '\\t'
                    pass 
                    self.match(9)


                elif alt20 == 3:
                    # /home/res/ANTLRv3_py.g:610:5: ( '\\r' )? '\\n'
                    pass 
                    # /home/res/ANTLRv3_py.g:610:5: ( '\\r' )?
                    alt19 = 2
                    LA19_0 = self.input.LA(1)

                    if (LA19_0 == 13) :
                        alt19 = 1
                    if alt19 == 1:
                        # /home/res/ANTLRv3_py.g:610:5: '\\r'
                        pass 
                        self.match(13)



                    self.match(10)


                else:
                    if cnt20 >= 1:
                        break #loop20

                    eee = EarlyExitException(20, self.input)
                    raise eee

                cnt20 += 1
            #action start
            _channel=HIDDEN;
            #action end



            self._state.type = _type
            self._state.channel = _channel

        finally:

            pass

    # $ANTLR end "WS"



    # $ANTLR start "WS_LOOP"
    def mWS_LOOP(self, ):

        try:
            # /home/res/ANTLRv3_py.g:617:2: ( ( WS | SL_COMMENT | ML_COMMENT )* )
            # /home/res/ANTLRv3_py.g:617:4: ( WS | SL_COMMENT | ML_COMMENT )*
            pass 
            # /home/res/ANTLRv3_py.g:617:4: ( WS | SL_COMMENT | ML_COMMENT )*
            while True: #loop21
                alt21 = 4
                LA21_0 = self.input.LA(1)

                if ((9 <= LA21_0 <= 10) or LA21_0 == 13 or LA21_0 == 32) :
                    alt21 = 1
                elif (LA21_0 == 47) :
                    LA21_3 = self.input.LA(2)

                    if (LA21_3 == 47) :
                        alt21 = 2
                    elif (LA21_3 == 42) :
                        alt21 = 3




                if alt21 == 1:
                    # /home/res/ANTLRv3_py.g:617:6: WS
                    pass 
                    self.mWS()


                elif alt21 == 2:
                    # /home/res/ANTLRv3_py.g:618:5: SL_COMMENT
                    pass 
                    self.mSL_COMMENT()


                elif alt21 == 3:
                    # /home/res/ANTLRv3_py.g:619:5: ML_COMMENT
                    pass 
                    self.mML_COMMENT()


                else:
                    break #loop21




        finally:

            pass

    # $ANTLR end "WS_LOOP"



    def mTokens(self):
        # /home/res/ANTLRv3_py.g:1:8: ( SCOPE | FRAGMENT | TREE_BEGIN | ROOT | BANG | RANGE | REWRITE | T__65 | T__66 | T__67 | T__68 | T__69 | T__70 | T__71 | T__72 | T__73 | T__74 | T__75 | T__76 | T__77 | T__78 | T__79 | T__80 | T__81 | T__82 | T__83 | T__84 | T__85 | T__86 | T__87 | T__88 | T__89 | T__90 | T__91 | T__92 | T__93 | SL_COMMENT | ML_COMMENT | CHAR_LITERAL | STRING_LITERAL | DOUBLE_QUOTE_STRING_LITERAL | DOUBLE_ANGLE_STRING_LITERAL | INT | ARG_ACTION | ACTION | TOKEN_REF | RULE_REF | OPTIONS | TOKENS | WS )
        alt22 = 50
        alt22 = self.dfa22.predict(self.input)
        if alt22 == 1:
            # /home/res/ANTLRv3_py.g:1:10: SCOPE
            pass 
            self.mSCOPE()


        elif alt22 == 2:
            # /home/res/ANTLRv3_py.g:1:16: FRAGMENT
            pass 
            self.mFRAGMENT()


        elif alt22 == 3:
            # /home/res/ANTLRv3_py.g:1:25: TREE_BEGIN
            pass 
            self.mTREE_BEGIN()


        elif alt22 == 4:
            # /home/res/ANTLRv3_py.g:1:36: ROOT
            pass 
            self.mROOT()


        elif alt22 == 5:
            # /home/res/ANTLRv3_py.g:1:41: BANG
            pass 
            self.mBANG()


        elif alt22 == 6:
            # /home/res/ANTLRv3_py.g:1:46: RANGE
            pass 
            self.mRANGE()


        elif alt22 == 7:
            # /home/res/ANTLRv3_py.g:1:52: REWRITE
            pass 
            self.mREWRITE()


        elif alt22 == 8:
            # /home/res/ANTLRv3_py.g:1:60: T__65
            pass 
            self.mT__65()


        elif alt22 == 9:
            # /home/res/ANTLRv3_py.g:1:66: T__66
            pass 
            self.mT__66()


        elif alt22 == 10:
            # /home/res/ANTLRv3_py.g:1:72: T__67
            pass 
            self.mT__67()


        elif alt22 == 11:
            # /home/res/ANTLRv3_py.g:1:78: T__68
            pass 
            self.mT__68()


        elif alt22 == 12:
            # /home/res/ANTLRv3_py.g:1:84: T__69
            pass 
            self.mT__69()


        elif alt22 == 13:
            # /home/res/ANTLRv3_py.g:1:90: T__70
            pass 
            self.mT__70()


        elif alt22 == 14:
            # /home/res/ANTLRv3_py.g:1:96: T__71
            pass 
            self.mT__71()


        elif alt22 == 15:
            # /home/res/ANTLRv3_py.g:1:102: T__72
            pass 
            self.mT__72()


        elif alt22 == 16:
            # /home/res/ANTLRv3_py.g:1:108: T__73
            pass 
            self.mT__73()


        elif alt22 == 17:
            # /home/res/ANTLRv3_py.g:1:114: T__74
            pass 
            self.mT__74()


        elif alt22 == 18:
            # /home/res/ANTLRv3_py.g:1:120: T__75
            pass 
            self.mT__75()


        elif alt22 == 19:
            # /home/res/ANTLRv3_py.g:1:126: T__76
            pass 
            self.mT__76()


        elif alt22 == 20:
            # /home/res/ANTLRv3_py.g:1:132: T__77
            pass 
            self.mT__77()


        elif alt22 == 21:
            # /home/res/ANTLRv3_py.g:1:138: T__78
            pass 
            self.mT__78()


        elif alt22 == 22:
            # /home/res/ANTLRv3_py.g:1:144: T__79
            pass 
            self.mT__79()


        elif alt22 == 23:
            # /home/res/ANTLRv3_py.g:1:150: T__80
            pass 
            self.mT__80()


        elif alt22 == 24:
            # /home/res/ANTLRv3_py.g:1:156: T__81
            pass 
            self.mT__81()


        elif alt22 == 25:
            # /home/res/ANTLRv3_py.g:1:162: T__82
            pass 
            self.mT__82()


        elif alt22 == 26:
            # /home/res/ANTLRv3_py.g:1:168: T__83
            pass 
            self.mT__83()


        elif alt22 == 27:
            # /home/res/ANTLRv3_py.g:1:174: T__84
            pass 
            self.mT__84()


        elif alt22 == 28:
            # /home/res/ANTLRv3_py.g:1:180: T__85
            pass 
            self.mT__85()


        elif alt22 == 29:
            # /home/res/ANTLRv3_py.g:1:186: T__86
            pass 
            self.mT__86()


        elif alt22 == 30:
            # /home/res/ANTLRv3_py.g:1:192: T__87
            pass 
            self.mT__87()


        elif alt22 == 31:
            # /home/res/ANTLRv3_py.g:1:198: T__88
            pass 
            self.mT__88()


        elif alt22 == 32:
            # /home/res/ANTLRv3_py.g:1:204: T__89
            pass 
            self.mT__89()


        elif alt22 == 33:
            # /home/res/ANTLRv3_py.g:1:210: T__90
            pass 
            self.mT__90()


        elif alt22 == 34:
            # /home/res/ANTLRv3_py.g:1:216: T__91
            pass 
            self.mT__91()


        elif alt22 == 35:
            # /home/res/ANTLRv3_py.g:1:222: T__92
            pass 
            self.mT__92()


        elif alt22 == 36:
            # /home/res/ANTLRv3_py.g:1:228: T__93
            pass 
            self.mT__93()


        elif alt22 == 37:
            # /home/res/ANTLRv3_py.g:1:234: SL_COMMENT
            pass 
            self.mSL_COMMENT()


        elif alt22 == 38:
            # /home/res/ANTLRv3_py.g:1:245: ML_COMMENT
            pass 
            self.mML_COMMENT()


        elif alt22 == 39:
            # /home/res/ANTLRv3_py.g:1:256: CHAR_LITERAL
            pass 
            self.mCHAR_LITERAL()


        elif alt22 == 40:
            # /home/res/ANTLRv3_py.g:1:269: STRING_LITERAL
            pass 
            self.mSTRING_LITERAL()


        elif alt22 == 41:
            # /home/res/ANTLRv3_py.g:1:284: DOUBLE_QUOTE_STRING_LITERAL
            pass 
            self.mDOUBLE_QUOTE_STRING_LITERAL()


        elif alt22 == 42:
            # /home/res/ANTLRv3_py.g:1:312: DOUBLE_ANGLE_STRING_LITERAL
            pass 
            self.mDOUBLE_ANGLE_STRING_LITERAL()


        elif alt22 == 43:
            # /home/res/ANTLRv3_py.g:1:340: INT
            pass 
            self.mINT()


        elif alt22 == 44:
            # /home/res/ANTLRv3_py.g:1:344: ARG_ACTION
            pass 
            self.mARG_ACTION()


        elif alt22 == 45:
            # /home/res/ANTLRv3_py.g:1:355: ACTION
            pass 
            self.mACTION()


        elif alt22 == 46:
            # /home/res/ANTLRv3_py.g:1:362: TOKEN_REF
            pass 
            self.mTOKEN_REF()


        elif alt22 == 47:
            # /home/res/ANTLRv3_py.g:1:372: RULE_REF
            pass 
            self.mRULE_REF()


        elif alt22 == 48:
            # /home/res/ANTLRv3_py.g:1:381: OPTIONS
            pass 
            self.mOPTIONS()


        elif alt22 == 49:
            # /home/res/ANTLRv3_py.g:1:389: TOKENS
            pass 
            self.mTOKENS()


        elif alt22 == 50:
            # /home/res/ANTLRv3_py.g:1:396: WS
            pass 
            self.mWS()







    # lookup tables for DFA #2

    DFA2_eot = DFA.unpack(
        u"\20\uffff\1\2\7\uffff\1\2\3\uffff"
        )

    DFA2_eof = DFA.unpack(
        u"\34\uffff"
        )

    DFA2_min = DFA.unpack(
        u"\2\0\1\uffff\21\0\1\uffff\6\0\1\uffff"
        )

    DFA2_max = DFA.unpack(
        u"\2\uffff\1\uffff\21\uffff\1\uffff\6\uffff\1\uffff"
        )

    DFA2_accept = DFA.unpack(
        u"\2\uffff\1\2\21\uffff\1\1\6\uffff\1\1"
        )

    DFA2_special = DFA.unpack(
        u"\1\21\1\12\1\uffff\1\11\1\14\1\13\1\0\1\2\1\3\1\24\1\25\1\27\1"
        u"\22\1\5\1\1\1\7\1\20\1\26\1\10\1\6\1\uffff\1\17\1\16\1\23\1\4\1"
        u"\30\1\15\1\uffff"
        )

            
    DFA2_transition = [
        DFA.unpack(u"\40\2\1\1\uffdf\2"),
        DFA.unpack(u"\44\2\1\3\uffdb\2"),
        DFA.unpack(u""),
        DFA.unpack(u"\101\2\1\4\uffbe\2"),
        DFA.unpack(u"\116\2\1\5\uffb1\2"),
        DFA.unpack(u"\124\2\1\6\uffab\2"),
        DFA.unpack(u"\114\2\1\7\uffb3\2"),
        DFA.unpack(u"\122\2\1\10\uffad\2"),
        DFA.unpack(u"\40\2\1\11\uffdf\2"),
        DFA.unpack(u"\163\2\1\12\uff8c\2"),
        DFA.unpack(u"\162\2\1\13\uff8d\2"),
        DFA.unpack(u"\143\2\1\14\uff9c\2"),
        DFA.unpack(u"\40\2\1\15\uffdf\2"),
        DFA.unpack(u"\42\2\1\16\uffdd\2"),
        DFA.unpack(u"\12\22\1\20\2\22\1\17\24\22\1\23\71\22\1\21\uffa3\22"),
        DFA.unpack(u"\12\24\1\20\ufff5\24"),
        DFA.unpack(u"\0\24"),
        DFA.unpack(u"\12\31\1\30\2\31\1\27\24\31\1\26\4\31\1\25\uffd8\31"),
        DFA.unpack(u"\12\22\1\20\2\22\1\17\24\22\1\23\71\22\1\21\uffa3\22"),
        DFA.unpack(u"\40\2\1\32\uffdf\2"),
        DFA.unpack(u""),
        DFA.unpack(u"\12\22\1\20\2\22\1\17\24\22\1\23\71\22\1\21\uffa3\22"),
        DFA.unpack(u"\12\22\1\20\2\22\1\17\24\22\1\23\71\22\1\21\uffa3\22"),
        DFA.unpack(u"\12\24\1\20\ufff5\24"),
        DFA.unpack(u"\0\24"),
        DFA.unpack(u"\12\22\1\20\2\22\1\17\24\22\1\23\71\22\1\21\uffa3\22"),
        DFA.unpack(u"\60\2\12\33\uffc6\2"),
        DFA.unpack(u"")
    ]

    # class definition for DFA #2

    class DFA2(DFA):
        pass


        def specialStateTransition(self_, s, input):
            # convince pylint that my self_ magic is ok ;)
            # pylint: disable-msg=E0213

            # pretend we are a member of the recognizer
            # thus semantic predicates can be evaluated
            self = self_.recognizer

            _s = s

            if s == 0: 
                LA2_6 = input.LA(1)

                s = -1
                if (LA2_6 == 76):
                    s = 7

                elif ((0 <= LA2_6 <= 75) or (77 <= LA2_6 <= 65535)):
                    s = 2

                if s >= 0:
                    return s
            elif s == 1: 
                LA2_14 = input.LA(1)

                s = -1
                if (LA2_14 == 13):
                    s = 15

                elif (LA2_14 == 10):
                    s = 16

                elif (LA2_14 == 92):
                    s = 17

                elif ((0 <= LA2_14 <= 9) or (11 <= LA2_14 <= 12) or (14 <= LA2_14 <= 33) or (35 <= LA2_14 <= 91) or (93 <= LA2_14 <= 65535)):
                    s = 18

                elif (LA2_14 == 34):
                    s = 19

                if s >= 0:
                    return s
            elif s == 2: 
                LA2_7 = input.LA(1)

                s = -1
                if (LA2_7 == 82):
                    s = 8

                elif ((0 <= LA2_7 <= 81) or (83 <= LA2_7 <= 65535)):
                    s = 2

                if s >= 0:
                    return s
            elif s == 3: 
                LA2_8 = input.LA(1)

                s = -1
                if (LA2_8 == 32):
                    s = 9

                elif ((0 <= LA2_8 <= 31) or (33 <= LA2_8 <= 65535)):
                    s = 2

                if s >= 0:
                    return s
            elif s == 4: 
                LA2_24 = input.LA(1)

                s = -1
                if ((0 <= LA2_24 <= 65535)):
                    s = 20

                else:
                    s = 2

                if s >= 0:
                    return s
            elif s == 5: 
                LA2_13 = input.LA(1)

                s = -1
                if ((0 <= LA2_13 <= 33) or (35 <= LA2_13 <= 65535)):
                    s = 2

                elif (LA2_13 == 34):
                    s = 14

                if s >= 0:
                    return s
            elif s == 6: 
                LA2_19 = input.LA(1)

                s = -1
                if ((0 <= LA2_19 <= 31) or (33 <= LA2_19 <= 65535)):
                    s = 2

                elif (LA2_19 == 32):
                    s = 26

                if s >= 0:
                    return s
            elif s == 7: 
                LA2_15 = input.LA(1)

                s = -1
                if (LA2_15 == 10):
                    s = 16

                elif ((0 <= LA2_15 <= 9) or (11 <= LA2_15 <= 65535)):
                    s = 20

                if s >= 0:
                    return s
            elif s == 8: 
                LA2_18 = input.LA(1)

                s = -1
                if (LA2_18 == 13):
                    s = 15

                elif (LA2_18 == 10):
                    s = 16

                elif (LA2_18 == 34):
                    s = 19

                elif (LA2_18 == 92):
                    s = 17

                elif ((0 <= LA2_18 <= 9) or (11 <= LA2_18 <= 12) or (14 <= LA2_18 <= 33) or (35 <= LA2_18 <= 91) or (93 <= LA2_18 <= 65535)):
                    s = 18

                if s >= 0:
                    return s
            elif s == 9: 
                LA2_3 = input.LA(1)

                s = -1
                if (LA2_3 == 65):
                    s = 4

                elif ((0 <= LA2_3 <= 64) or (66 <= LA2_3 <= 65535)):
                    s = 2

                if s >= 0:
                    return s
            elif s == 10: 
                LA2_1 = input.LA(1)

                s = -1
                if (LA2_1 == 36):
                    s = 3

                elif ((0 <= LA2_1 <= 35) or (37 <= LA2_1 <= 65535)):
                    s = 2

                if s >= 0:
                    return s
            elif s == 11: 
                LA2_5 = input.LA(1)

                s = -1
                if (LA2_5 == 84):
                    s = 6

                elif ((0 <= LA2_5 <= 83) or (85 <= LA2_5 <= 65535)):
                    s = 2

                if s >= 0:
                    return s
            elif s == 12: 
                LA2_4 = input.LA(1)

                s = -1
                if (LA2_4 == 78):
                    s = 5

                elif ((0 <= LA2_4 <= 77) or (79 <= LA2_4 <= 65535)):
                    s = 2

                if s >= 0:
                    return s
            elif s == 13: 
                LA2_26 = input.LA(1)

                s = -1
                if ((0 <= LA2_26 <= 47) or (58 <= LA2_26 <= 65535)):
                    s = 2

                elif ((48 <= LA2_26 <= 57)):
                    s = 27

                if s >= 0:
                    return s
            elif s == 14: 
                LA2_22 = input.LA(1)

                s = -1
                if (LA2_22 == 13):
                    s = 15

                elif (LA2_22 == 10):
                    s = 16

                elif (LA2_22 == 34):
                    s = 19

                elif (LA2_22 == 92):
                    s = 17

                elif ((0 <= LA2_22 <= 9) or (11 <= LA2_22 <= 12) or (14 <= LA2_22 <= 33) or (35 <= LA2_22 <= 91) or (93 <= LA2_22 <= 65535)):
                    s = 18

                if s >= 0:
                    return s
            elif s == 15: 
                LA2_21 = input.LA(1)

                s = -1
                if (LA2_21 == 13):
                    s = 15

                elif (LA2_21 == 10):
                    s = 16

                elif (LA2_21 == 34):
                    s = 19

                elif (LA2_21 == 92):
                    s = 17

                elif ((0 <= LA2_21 <= 9) or (11 <= LA2_21 <= 12) or (14 <= LA2_21 <= 33) or (35 <= LA2_21 <= 91) or (93 <= LA2_21 <= 65535)):
                    s = 18

                if s >= 0:
                    return s
            elif s == 16: 
                LA2_16 = input.LA(1)

                s = -1
                if ((0 <= LA2_16 <= 65535)):
                    s = 20

                else:
                    s = 2

                if s >= 0:
                    return s
            elif s == 17: 
                LA2_0 = input.LA(1)

                s = -1
                if (LA2_0 == 32):
                    s = 1

                elif ((0 <= LA2_0 <= 31) or (33 <= LA2_0 <= 65535)):
                    s = 2

                if s >= 0:
                    return s
            elif s == 18: 
                LA2_12 = input.LA(1)

                s = -1
                if (LA2_12 == 32):
                    s = 13

                elif ((0 <= LA2_12 <= 31) or (33 <= LA2_12 <= 65535)):
                    s = 2

                if s >= 0:
                    return s
            elif s == 19: 
                LA2_23 = input.LA(1)

                s = -1
                if ((0 <= LA2_23 <= 9) or (11 <= LA2_23 <= 65535)):
                    s = 20

                elif (LA2_23 == 10):
                    s = 16

                if s >= 0:
                    return s
            elif s == 20: 
                LA2_9 = input.LA(1)

                s = -1
                if ((0 <= LA2_9 <= 114) or (116 <= LA2_9 <= 65535)):
                    s = 2

                elif (LA2_9 == 115):
                    s = 10

                if s >= 0:
                    return s
            elif s == 21: 
                LA2_10 = input.LA(1)

                s = -1
                if (LA2_10 == 114):
                    s = 11

                elif ((0 <= LA2_10 <= 113) or (115 <= LA2_10 <= 65535)):
                    s = 2

                if s >= 0:
                    return s
            elif s == 22: 
                LA2_17 = input.LA(1)

                s = -1
                if (LA2_17 == 39):
                    s = 21

                elif (LA2_17 == 34):
                    s = 22

                elif (LA2_17 == 13):
                    s = 23

                elif (LA2_17 == 10):
                    s = 24

                elif ((0 <= LA2_17 <= 9) or (11 <= LA2_17 <= 12) or (14 <= LA2_17 <= 33) or (35 <= LA2_17 <= 38) or (40 <= LA2_17 <= 65535)):
                    s = 25

                if s >= 0:
                    return s
            elif s == 23: 
                LA2_11 = input.LA(1)

                s = -1
                if (LA2_11 == 99):
                    s = 12

                elif ((0 <= LA2_11 <= 98) or (100 <= LA2_11 <= 65535)):
                    s = 2

                if s >= 0:
                    return s
            elif s == 24: 
                LA2_25 = input.LA(1)

                s = -1
                if (LA2_25 == 34):
                    s = 19

                elif (LA2_25 == 92):
                    s = 17

                elif (LA2_25 == 13):
                    s = 15

                elif (LA2_25 == 10):
                    s = 16

                elif ((0 <= LA2_25 <= 9) or (11 <= LA2_25 <= 12) or (14 <= LA2_25 <= 33) or (35 <= LA2_25 <= 91) or (93 <= LA2_25 <= 65535)):
                    s = 18

                if s >= 0:
                    return s

            nvae = NoViableAltException(self_.getDescription(), 2, _s, input)
            self_.error(nvae)
            raise nvae
    # lookup tables for DFA #9

    DFA9_eot = DFA.unpack(
        u"\12\uffff\1\13\2\uffff"
        )

    DFA9_eof = DFA.unpack(
        u"\15\uffff"
        )

    DFA9_min = DFA.unpack(
        u"\1\0\11\uffff\1\60\2\uffff"
        )

    DFA9_max = DFA.unpack(
        u"\1\uffff\11\uffff\1\146\2\uffff"
        )

    DFA9_accept = DFA.unpack(
        u"\1\uffff\1\1\1\2\1\3\1\4\1\5\1\6\1\7\1\10\1\11\1\uffff\1\13\1\12"
        )

    DFA9_special = DFA.unpack(
        u"\1\0\14\uffff"
        )

            
    DFA9_transition = [
        DFA.unpack(u"\42\13\1\6\4\13\1\7\26\13\1\11\35\13\1\10\5\13\1\4\3"
        u"\13\1\5\7\13\1\1\3\13\1\2\1\13\1\3\1\12\uff8a\13"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\12\14\7\uffff\6\14\32\uffff\6\14"),
        DFA.unpack(u""),
        DFA.unpack(u"")
    ]

    # class definition for DFA #9

    class DFA9(DFA):
        pass


        def specialStateTransition(self_, s, input):
            # convince pylint that my self_ magic is ok ;)
            # pylint: disable-msg=E0213

            # pretend we are a member of the recognizer
            # thus semantic predicates can be evaluated
            self = self_.recognizer

            _s = s

            if s == 0: 
                LA9_0 = input.LA(1)

                s = -1
                if (LA9_0 == 110):
                    s = 1

                elif (LA9_0 == 114):
                    s = 2

                elif (LA9_0 == 116):
                    s = 3

                elif (LA9_0 == 98):
                    s = 4

                elif (LA9_0 == 102):
                    s = 5

                elif (LA9_0 == 34):
                    s = 6

                elif (LA9_0 == 39):
                    s = 7

                elif (LA9_0 == 92):
                    s = 8

                elif (LA9_0 == 62):
                    s = 9

                elif (LA9_0 == 117):
                    s = 10

                elif ((0 <= LA9_0 <= 33) or (35 <= LA9_0 <= 38) or (40 <= LA9_0 <= 61) or (63 <= LA9_0 <= 91) or (93 <= LA9_0 <= 97) or (99 <= LA9_0 <= 101) or (103 <= LA9_0 <= 109) or (111 <= LA9_0 <= 113) or LA9_0 == 115 or (118 <= LA9_0 <= 65535)):
                    s = 11

                if s >= 0:
                    return s

            nvae = NoViableAltException(self_.getDescription(), 9, _s, input)
            self_.error(nvae)
            raise nvae
    # lookup tables for DFA #13

    DFA13_eot = DFA.unpack(
        u"\34\uffff"
        )

    DFA13_eof = DFA.unpack(
        u"\34\uffff"
        )

    DFA13_min = DFA.unpack(
        u"\1\0\2\uffff\3\0\26\uffff"
        )

    DFA13_max = DFA.unpack(
        u"\1\uffff\2\uffff\3\uffff\26\uffff"
        )

    DFA13_accept = DFA.unpack(
        u"\1\uffff\1\7\1\1\3\uffff\1\6\1\2\1\3\5\uffff\7\4\6\5\1\uffff"
        )

    DFA13_special = DFA.unpack(
        u"\1\0\2\uffff\1\1\1\2\1\3\26\uffff"
        )

            
    DFA13_transition = [
        DFA.unpack(u"\42\6\1\4\4\6\1\5\7\6\1\3\113\6\1\2\1\6\1\1\uff82\6"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\52\6\1\10\4\6\1\7\uffd0\6"),
        DFA.unpack(u"\42\24\1\20\4\24\1\23\7\24\1\22\54\24\1\16\36\24\1"
        u"\21\1\24\1\17\uff82\24"),
        DFA.unpack(u"\42\32\1\31\4\32\1\6\7\32\1\30\54\32\1\25\36\32\1\27"
        u"\1\32\1\26\uff82\32"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"")
    ]

    # class definition for DFA #13

    class DFA13(DFA):
        pass


        def specialStateTransition(self_, s, input):
            # convince pylint that my self_ magic is ok ;)
            # pylint: disable-msg=E0213

            # pretend we are a member of the recognizer
            # thus semantic predicates can be evaluated
            self = self_.recognizer

            _s = s

            if s == 0: 
                LA13_0 = input.LA(1)

                s = -1
                if (LA13_0 == 125):
                    s = 1

                elif (LA13_0 == 123):
                    s = 2

                elif (LA13_0 == 47):
                    s = 3

                elif (LA13_0 == 34):
                    s = 4

                elif (LA13_0 == 39):
                    s = 5

                elif ((0 <= LA13_0 <= 33) or (35 <= LA13_0 <= 38) or (40 <= LA13_0 <= 46) or (48 <= LA13_0 <= 122) or LA13_0 == 124 or (126 <= LA13_0 <= 65535)):
                    s = 6

                if s >= 0:
                    return s
            elif s == 1: 
                LA13_3 = input.LA(1)

                s = -1
                if (LA13_3 == 47):
                    s = 7

                elif (LA13_3 == 42):
                    s = 8

                elif ((0 <= LA13_3 <= 41) or (43 <= LA13_3 <= 46) or (48 <= LA13_3 <= 65535)):
                    s = 6

                if s >= 0:
                    return s
            elif s == 2: 
                LA13_4 = input.LA(1)

                s = -1
                if (LA13_4 == 92):
                    s = 14

                elif (LA13_4 == 125):
                    s = 15

                elif (LA13_4 == 34):
                    s = 16

                elif (LA13_4 == 123):
                    s = 17

                elif (LA13_4 == 47):
                    s = 18

                elif (LA13_4 == 39):
                    s = 19

                elif ((0 <= LA13_4 <= 33) or (35 <= LA13_4 <= 38) or (40 <= LA13_4 <= 46) or (48 <= LA13_4 <= 91) or (93 <= LA13_4 <= 122) or LA13_4 == 124 or (126 <= LA13_4 <= 65535)):
                    s = 20

                if s >= 0:
                    return s
            elif s == 3: 
                LA13_5 = input.LA(1)

                s = -1
                if (LA13_5 == 92):
                    s = 21

                elif (LA13_5 == 125):
                    s = 22

                elif (LA13_5 == 123):
                    s = 23

                elif (LA13_5 == 47):
                    s = 24

                elif (LA13_5 == 34):
                    s = 25

                elif ((0 <= LA13_5 <= 33) or (35 <= LA13_5 <= 38) or (40 <= LA13_5 <= 46) or (48 <= LA13_5 <= 91) or (93 <= LA13_5 <= 122) or LA13_5 == 124 or (126 <= LA13_5 <= 65535)):
                    s = 26

                elif (LA13_5 == 39):
                    s = 6

                if s >= 0:
                    return s

            nvae = NoViableAltException(self_.getDescription(), 13, _s, input)
            self_.error(nvae)
            raise nvae
    # lookup tables for DFA #22

    DFA22_eot = DFA.unpack(
        u"\1\uffff\2\44\1\52\1\uffff\1\54\1\uffff\4\44\2\uffff\1\66\1\uffff"
        u"\1\70\1\uffff\1\44\4\uffff\1\44\1\74\13\uffff\1\44\2\uffff\3\44"
        u"\4\uffff\10\44\4\uffff\2\44\6\uffff\17\44\15\uffff\11\44\1\167"
        u"\5\44\2\uffff\1\44\1\177\2\44\1\u0082\4\44\1\uffff\4\44\1\u008b"
        u"\1\uffff\1\44\1\uffff\2\44\1\uffff\1\u0090\2\44\1\u0093\1\u0094"
        u"\3\44\2\uffff\2\44\1\u009b\1\uffff\1\44\1\u009d\3\uffff\1\u009e"
        u"\1\u009f\1\uffff\1\44\1\u00a1\1\uffff\1\44\5\uffff\1\u00a3\1\uffff"
        )

    DFA22_eof = DFA.unpack(
        u"\u00a4\uffff"
        )

    DFA22_min = DFA.unpack(
        u"\1\11\1\143\1\151\1\50\1\uffff\1\56\1\uffff\1\145\1\141\1\150\1"
        u"\162\2\uffff\1\76\1\uffff\1\72\1\uffff\1\145\4\uffff\1\141\1\75"
        u"\3\uffff\1\52\1\0\6\uffff\1\160\2\uffff\1\157\1\141\1\156\4\uffff"
        u"\1\170\1\162\1\151\1\142\1\145\1\162\1\153\1\141\4\uffff\2\164"
        u"\4\uffff\2\0\1\164\1\160\1\147\1\141\1\145\1\163\1\164\1\166\1"
        u"\154\1\145\1\157\1\145\1\155\1\165\1\143\13\0\2\uffff\1\151\1\145"
        u"\1\155\1\154\1\162\2\145\1\141\1\151\1\60\1\167\1\156\1\155\1\162"
        u"\1\150\1\0\1\uffff\1\157\1\60\1\145\1\154\1\60\1\162\1\143\1\164"
        u"\1\143\1\uffff\2\163\1\141\1\156\1\60\1\0\1\156\1\uffff\1\156\1"
        u"\171\1\uffff\1\60\1\164\1\145\2\60\1\11\1\162\1\163\1\uffff\1\0"
        u"\1\163\1\164\1\60\1\uffff\1\145\1\60\3\uffff\2\60\1\0\1\11\1\60"
        u"\1\uffff\1\144\5\uffff\1\60\1\uffff"
        )

    DFA22_max = DFA.unpack(
        u"\1\176\1\143\1\162\1\50\1\uffff\1\56\1\uffff\1\145\1\165\2\162"
        u"\2\uffff\1\76\1\uffff\1\72\1\uffff\1\145\4\uffff\1\141\1\75\3\uffff"
        u"\1\57\1\uffff\6\uffff\1\160\2\uffff\1\157\1\141\1\156\4\uffff\1"
        u"\170\1\162\1\157\1\142\1\145\1\162\1\153\1\141\4\uffff\2\164\4"
        u"\uffff\2\uffff\1\164\1\160\1\147\1\141\1\145\1\163\1\164\1\166"
        u"\1\154\1\145\1\157\1\145\1\155\1\165\1\143\13\uffff\2\uffff\1\151"
        u"\1\145\1\155\1\154\1\162\2\145\1\141\1\151\1\172\1\167\1\156\1"
        u"\155\1\162\1\150\1\uffff\1\uffff\1\157\1\172\1\145\1\154\1\172"
        u"\1\162\1\143\1\164\1\143\1\uffff\2\163\1\141\1\156\1\172\1\uffff"
        u"\1\156\1\uffff\1\156\1\171\1\uffff\1\172\1\164\1\145\2\172\1\173"
        u"\1\162\1\163\1\uffff\1\uffff\1\163\1\164\1\172\1\uffff\1\145\1"
        u"\172\3\uffff\2\172\1\uffff\1\173\1\172\1\uffff\1\144\5\uffff\1"
        u"\172\1\uffff"
        )

    DFA22_accept = DFA.unpack(
        u"\4\uffff\1\5\1\uffff\1\7\4\uffff\1\14\1\15\1\uffff\1\17\1\uffff"
        u"\1\21\1\uffff\1\30\1\31\1\32\1\33\2\uffff\1\40\1\41\1\44\2\uffff"
        u"\1\51\1\52\1\53\1\54\1\55\1\56\1\uffff\1\57\1\62\3\uffff\1\3\1"
        u"\4\1\6\1\43\10\uffff\1\37\1\16\1\20\1\26\2\uffff\1\36\1\42\1\45"
        u"\1\46\34\uffff\1\47\1\50\20\uffff\1\47\11\uffff\1\12\7\uffff\1"
        u"\1\2\uffff\1\10\10\uffff\1\34\4\uffff\1\11\2\uffff\1\23\1\27\1"
        u"\61\5\uffff\1\35\1\uffff\1\24\1\13\1\25\1\60\1\2\1\uffff\1\22"
        )

    DFA22_special = DFA.unpack(
        u"\34\uffff\1\4\42\uffff\1\0\1\5\17\uffff\1\3\1\2\1\1\1\16\1\14\1"
        u"\21\1\20\1\12\1\11\1\7\1\6\21\uffff\1\13\20\uffff\1\15\16\uffff"
        u"\1\17\13\uffff\1\10\13\uffff"
        )

            
    DFA22_transition = [
        DFA.unpack(u"\2\45\2\uffff\1\45\22\uffff\1\45\1\4\1\35\1\uffff\1"
        u"\32\2\uffff\1\34\1\23\1\25\1\20\1\27\1\22\1\6\1\5\1\33\12\37\1"
        u"\17\1\13\1\36\1\15\1\uffff\1\31\1\16\32\42\1\40\2\uffff\1\3\2\uffff"
        u"\2\44\1\26\2\44\1\2\1\12\4\44\1\7\2\44\1\43\1\10\1\44\1\21\1\1"
        u"\1\11\6\44\1\41\1\24\1\14\1\30"),
        DFA.unpack(u"\1\46"),
        DFA.unpack(u"\1\50\10\uffff\1\47"),
        DFA.unpack(u"\1\51"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\53"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\55"),
        DFA.unpack(u"\1\56\20\uffff\1\57\2\uffff\1\60"),
        DFA.unpack(u"\1\62\6\uffff\1\63\2\uffff\1\61"),
        DFA.unpack(u"\1\64"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\65"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\67"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\71"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\72"),
        DFA.unpack(u"\1\73"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\76\4\uffff\1\75"),
        DFA.unpack(u"\47\100\1\uffff\64\100\1\77\uffa3\100"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\101"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\102"),
        DFA.unpack(u"\1\103"),
        DFA.unpack(u"\1\104"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\105"),
        DFA.unpack(u"\1\106"),
        DFA.unpack(u"\1\110\5\uffff\1\107"),
        DFA.unpack(u"\1\111"),
        DFA.unpack(u"\1\112"),
        DFA.unpack(u"\1\113"),
        DFA.unpack(u"\1\114"),
        DFA.unpack(u"\1\115"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\116"),
        DFA.unpack(u"\1\117"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\42\132\1\125\4\132\1\126\26\132\1\130\35\132\1\127"
        u"\5\132\1\123\3\132\1\124\7\132\1\120\3\132\1\121\1\132\1\122\1"
        u"\131\uff8a\132"),
        DFA.unpack(u"\47\134\1\133\uffd8\134"),
        DFA.unpack(u"\1\135"),
        DFA.unpack(u"\1\136"),
        DFA.unpack(u"\1\137"),
        DFA.unpack(u"\1\140"),
        DFA.unpack(u"\1\141"),
        DFA.unpack(u"\1\142"),
        DFA.unpack(u"\1\143"),
        DFA.unpack(u"\1\144"),
        DFA.unpack(u"\1\145"),
        DFA.unpack(u"\1\146"),
        DFA.unpack(u"\1\147"),
        DFA.unpack(u"\1\150"),
        DFA.unpack(u"\1\151"),
        DFA.unpack(u"\1\152"),
        DFA.unpack(u"\1\153"),
        DFA.unpack(u"\47\134\1\133\uffd8\134"),
        DFA.unpack(u"\47\134\1\133\uffd8\134"),
        DFA.unpack(u"\47\134\1\133\uffd8\134"),
        DFA.unpack(u"\47\134\1\133\uffd8\134"),
        DFA.unpack(u"\47\134\1\133\uffd8\134"),
        DFA.unpack(u"\47\134\1\133\uffd8\134"),
        DFA.unpack(u"\47\134\1\133\uffd8\134"),
        DFA.unpack(u"\47\134\1\133\uffd8\134"),
        DFA.unpack(u"\47\134\1\133\uffd8\134"),
        DFA.unpack(u"\47\134\1\133\10\134\12\154\7\134\6\154\32\134\6\154"
        u"\uff99\134"),
        DFA.unpack(u"\47\134\1\133\uffd8\134"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\156"),
        DFA.unpack(u"\1\157"),
        DFA.unpack(u"\1\160"),
        DFA.unpack(u"\1\161"),
        DFA.unpack(u"\1\162"),
        DFA.unpack(u"\1\163"),
        DFA.unpack(u"\1\164"),
        DFA.unpack(u"\1\165"),
        DFA.unpack(u"\1\166"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\1\170"),
        DFA.unpack(u"\1\171"),
        DFA.unpack(u"\1\172"),
        DFA.unpack(u"\1\173"),
        DFA.unpack(u"\1\174"),
        DFA.unpack(u"\60\134\12\175\7\134\6\175\32\134\6\175\uff99\134"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\176"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\1\u0080"),
        DFA.unpack(u"\1\u0081"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\1\u0083"),
        DFA.unpack(u"\1\u0084"),
        DFA.unpack(u"\1\u0085"),
        DFA.unpack(u"\1\u0086"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u0087"),
        DFA.unpack(u"\1\u0088"),
        DFA.unpack(u"\1\u0089"),
        DFA.unpack(u"\1\u008a"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\60\134\12\u008c\7\134\6\u008c\32\134\6\u008c\uff99"
        u"\134"),
        DFA.unpack(u"\1\u008d"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u008e"),
        DFA.unpack(u"\1\u008f"),
        DFA.unpack(u""),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\1\u0091"),
        DFA.unpack(u"\1\u0092"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\2\u0095\2\uffff\1\u0095\22\uffff\1\u0095\16\uffff"
        u"\1\u0095\113\uffff\1\u0095"),
        DFA.unpack(u"\1\u0096"),
        DFA.unpack(u"\1\u0097"),
        DFA.unpack(u""),
        DFA.unpack(u"\60\134\12\u0098\7\134\6\u0098\32\134\6\u0098\uff99"
        u"\134"),
        DFA.unpack(u"\1\u0099"),
        DFA.unpack(u"\1\u009a"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u009c"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\47\134\1\133\uffd8\134"),
        DFA.unpack(u"\2\u00a0\2\uffff\1\u00a0\22\uffff\1\u00a0\16\uffff"
        u"\1\u00a0\113\uffff\1\u00a0"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00a2"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"")
    ]

    # class definition for DFA #22

    class DFA22(DFA):
        pass


        def specialStateTransition(self_, s, input):
            # convince pylint that my self_ magic is ok ;)
            # pylint: disable-msg=E0213

            # pretend we are a member of the recognizer
            # thus semantic predicates can be evaluated
            self = self_.recognizer

            _s = s

            if s == 0: 
                LA22_63 = input.LA(1)

                s = -1
                if (LA22_63 == 110):
                    s = 80

                elif (LA22_63 == 114):
                    s = 81

                elif (LA22_63 == 116):
                    s = 82

                elif (LA22_63 == 98):
                    s = 83

                elif (LA22_63 == 102):
                    s = 84

                elif (LA22_63 == 34):
                    s = 85

                elif (LA22_63 == 39):
                    s = 86

                elif (LA22_63 == 92):
                    s = 87

                elif (LA22_63 == 62):
                    s = 88

                elif (LA22_63 == 117):
                    s = 89

                elif ((0 <= LA22_63 <= 33) or (35 <= LA22_63 <= 38) or (40 <= LA22_63 <= 61) or (63 <= LA22_63 <= 91) or (93 <= LA22_63 <= 97) or (99 <= LA22_63 <= 101) or (103 <= LA22_63 <= 109) or (111 <= LA22_63 <= 113) or LA22_63 == 115 or (118 <= LA22_63 <= 65535)):
                    s = 90

                if s >= 0:
                    return s
            elif s == 1: 
                LA22_82 = input.LA(1)

                s = -1
                if (LA22_82 == 39):
                    s = 91

                elif ((0 <= LA22_82 <= 38) or (40 <= LA22_82 <= 65535)):
                    s = 92

                if s >= 0:
                    return s
            elif s == 2: 
                LA22_81 = input.LA(1)

                s = -1
                if ((0 <= LA22_81 <= 38) or (40 <= LA22_81 <= 65535)):
                    s = 92

                elif (LA22_81 == 39):
                    s = 91

                if s >= 0:
                    return s
            elif s == 3: 
                LA22_80 = input.LA(1)

                s = -1
                if ((0 <= LA22_80 <= 38) or (40 <= LA22_80 <= 65535)):
                    s = 92

                elif (LA22_80 == 39):
                    s = 91

                if s >= 0:
                    return s
            elif s == 4: 
                LA22_28 = input.LA(1)

                s = -1
                if (LA22_28 == 92):
                    s = 63

                elif ((0 <= LA22_28 <= 38) or (40 <= LA22_28 <= 91) or (93 <= LA22_28 <= 65535)):
                    s = 64

                if s >= 0:
                    return s
            elif s == 5: 
                LA22_64 = input.LA(1)

                s = -1
                if (LA22_64 == 39):
                    s = 91

                elif ((0 <= LA22_64 <= 38) or (40 <= LA22_64 <= 65535)):
                    s = 92

                if s >= 0:
                    return s
            elif s == 6: 
                LA22_90 = input.LA(1)

                s = -1
                if ((0 <= LA22_90 <= 38) or (40 <= LA22_90 <= 65535)):
                    s = 92

                elif (LA22_90 == 39):
                    s = 91

                if s >= 0:
                    return s
            elif s == 7: 
                LA22_89 = input.LA(1)

                s = -1
                if ((0 <= LA22_89 <= 38) or (40 <= LA22_89 <= 47) or (58 <= LA22_89 <= 64) or (71 <= LA22_89 <= 96) or (103 <= LA22_89 <= 65535)):
                    s = 92

                elif ((48 <= LA22_89 <= 57) or (65 <= LA22_89 <= 70) or (97 <= LA22_89 <= 102)):
                    s = 108

                elif (LA22_89 == 39):
                    s = 91

                if s >= 0:
                    return s
            elif s == 8: 
                LA22_152 = input.LA(1)

                s = -1
                if (LA22_152 == 39):
                    s = 91

                elif ((0 <= LA22_152 <= 38) or (40 <= LA22_152 <= 65535)):
                    s = 92

                if s >= 0:
                    return s
            elif s == 9: 
                LA22_88 = input.LA(1)

                s = -1
                if ((0 <= LA22_88 <= 38) or (40 <= LA22_88 <= 65535)):
                    s = 92

                elif (LA22_88 == 39):
                    s = 91

                if s >= 0:
                    return s
            elif s == 10: 
                LA22_87 = input.LA(1)

                s = -1
                if ((0 <= LA22_87 <= 38) or (40 <= LA22_87 <= 65535)):
                    s = 92

                elif (LA22_87 == 39):
                    s = 91

                if s >= 0:
                    return s
            elif s == 11: 
                LA22_108 = input.LA(1)

                s = -1
                if ((48 <= LA22_108 <= 57) or (65 <= LA22_108 <= 70) or (97 <= LA22_108 <= 102)):
                    s = 125

                elif ((0 <= LA22_108 <= 47) or (58 <= LA22_108 <= 64) or (71 <= LA22_108 <= 96) or (103 <= LA22_108 <= 65535)):
                    s = 92

                if s >= 0:
                    return s
            elif s == 12: 
                LA22_84 = input.LA(1)

                s = -1
                if (LA22_84 == 39):
                    s = 91

                elif ((0 <= LA22_84 <= 38) or (40 <= LA22_84 <= 65535)):
                    s = 92

                if s >= 0:
                    return s
            elif s == 13: 
                LA22_125 = input.LA(1)

                s = -1
                if ((48 <= LA22_125 <= 57) or (65 <= LA22_125 <= 70) or (97 <= LA22_125 <= 102)):
                    s = 140

                elif ((0 <= LA22_125 <= 47) or (58 <= LA22_125 <= 64) or (71 <= LA22_125 <= 96) or (103 <= LA22_125 <= 65535)):
                    s = 92

                if s >= 0:
                    return s
            elif s == 14: 
                LA22_83 = input.LA(1)

                s = -1
                if (LA22_83 == 39):
                    s = 91

                elif ((0 <= LA22_83 <= 38) or (40 <= LA22_83 <= 65535)):
                    s = 92

                if s >= 0:
                    return s
            elif s == 15: 
                LA22_140 = input.LA(1)

                s = -1
                if ((0 <= LA22_140 <= 47) or (58 <= LA22_140 <= 64) or (71 <= LA22_140 <= 96) or (103 <= LA22_140 <= 65535)):
                    s = 92

                elif ((48 <= LA22_140 <= 57) or (65 <= LA22_140 <= 70) or (97 <= LA22_140 <= 102)):
                    s = 152

                if s >= 0:
                    return s
            elif s == 16: 
                LA22_86 = input.LA(1)

                s = -1
                if ((0 <= LA22_86 <= 38) or (40 <= LA22_86 <= 65535)):
                    s = 92

                elif (LA22_86 == 39):
                    s = 91

                if s >= 0:
                    return s
            elif s == 17: 
                LA22_85 = input.LA(1)

                s = -1
                if (LA22_85 == 39):
                    s = 91

                elif ((0 <= LA22_85 <= 38) or (40 <= LA22_85 <= 65535)):
                    s = 92

                if s >= 0:
                    return s

            nvae = NoViableAltException(self_.getDescription(), 22, _s, input)
            self_.error(nvae)
            raise nvae
 



def main(argv, stdin=sys.stdin, stdout=sys.stdout, stderr=sys.stderr):
    from antlr3.main import LexerMain
    main = LexerMain(ANTLRv3_pyLexer)
    main.stdin = stdin
    main.stdout = stdout
    main.stderr = stderr
    main.execute(argv)


if __name__ == '__main__':
    main(sys.argv)
