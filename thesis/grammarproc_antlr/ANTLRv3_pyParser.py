# $ANTLR 3.2 Sep 23, 2009 12:02:23 /home/res/ANTLRv3_py.g 2009-11-28 18:18:11

import sys
from antlr3 import *
from antlr3.compat import set, frozenset

from antlr3.tree import *



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
RULE=7
T__82=82
T__83=83
ACTION_ESC=62
PARSER_GRAMMAR=25
SRC=52
CHAR_RANGE=14
INT=47
EPSILON=15
T__85=85
T__84=84
T__87=87
T__86=86
T__89=89
REWRITE=40
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



class rule_scope(object):
    def __init__(self):
        self.name = None


class ANTLRv3_pyParser(Parser):
    grammarFileName = "/home/res/ANTLRv3_py.g"
    #antlr_version = version_str_to_tuple("3.2 Sep 23, 2009 12:02:23")
    #antlr_version_str = "3.2 Sep 23, 2009 12:02:23"
    antlr_version = version_str_to_tuple("3.1")
    antlr_version_str = "3.1"
    tokenNames = tokenNames

    def __init__(self, input, state=None, *args, **kwargs):
        if state is None:
            state = RecognizerSharedState()

        super(ANTLRv3_pyParser, self).__init__(input, state, *args, **kwargs)

        self.dfa46 = self.DFA46(
            self, 46,
            eot = self.DFA46_eot,
            eof = self.DFA46_eof,
            min = self.DFA46_min,
            max = self.DFA46_max,
            accept = self.DFA46_accept,
            special = self.DFA46_special,
            transition = self.DFA46_transition
            )

        self.dfa64 = self.DFA64(
            self, 64,
            eot = self.DFA64_eot,
            eof = self.DFA64_eof,
            min = self.DFA64_min,
            max = self.DFA64_max,
            accept = self.DFA64_accept,
            special = self.DFA64_special,
            transition = self.DFA64_transition
            )

        self.dfa67 = self.DFA67(
            self, 67,
            eot = self.DFA67_eot,
            eof = self.DFA67_eof,
            min = self.DFA67_min,
            max = self.DFA67_max,
            accept = self.DFA67_accept,
            special = self.DFA67_special,
            transition = self.DFA67_transition
            )

        self.dfa72 = self.DFA72(
            self, 72,
            eot = self.DFA72_eot,
            eof = self.DFA72_eof,
            min = self.DFA72_min,
            max = self.DFA72_max,
            accept = self.DFA72_accept,
            special = self.DFA72_special,
            transition = self.DFA72_transition
            )


	self.rule_stack = []


               
        self.gtype = None;




        self._adaptor = None
        self.adaptor = CommonTreeAdaptor()
                


        
    def getTreeAdaptor(self):
        return self._adaptor

    def setTreeAdaptor(self, adaptor):
        self._adaptor = adaptor

    adaptor = property(getTreeAdaptor, setTreeAdaptor)

              
    #	int gtype;


    class grammarDef_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.grammarDef_return, self).__init__()

            self.tree = None




    # $ANTLR start "grammarDef"
    # /home/res/ANTLRv3_py.g:87:1: grammarDef : ( DOC_COMMENT )? ( 'lexer' | 'parser' | 'tree' | ) g= 'grammar' id ';' ( optionsSpec )? ( tokensSpec )? ( attrScope )* ( action )* ( rule )+ EOF -> ^( id ( DOC_COMMENT )? ( optionsSpec )? ( tokensSpec )? ( attrScope )* ( action )* ( rule )+ ) ;
    def grammarDef(self, ):

        retval = self.grammarDef_return()
        retval.start = self.input.LT(1)

        root_0 = None

        g = None
        DOC_COMMENT1 = None
        string_literal2 = None
        string_literal3 = None
        string_literal4 = None
        char_literal6 = None
        EOF12 = None
        id5 = None

        optionsSpec7 = None

        tokensSpec8 = None

        attrScope9 = None

        action10 = None

        rule11 = None


        g_tree = None
        DOC_COMMENT1_tree = None
        string_literal2_tree = None
        string_literal3_tree = None
        string_literal4_tree = None
        char_literal6_tree = None
        EOF12_tree = None
        stream_67 = RewriteRuleTokenStream(self._adaptor, "token 67")
        stream_DOC_COMMENT = RewriteRuleTokenStream(self._adaptor, "token DOC_COMMENT")
        stream_66 = RewriteRuleTokenStream(self._adaptor, "token 66")
        stream_69 = RewriteRuleTokenStream(self._adaptor, "token 69")
        stream_68 = RewriteRuleTokenStream(self._adaptor, "token 68")
        stream_65 = RewriteRuleTokenStream(self._adaptor, "token 65")
        stream_EOF = RewriteRuleTokenStream(self._adaptor, "token EOF")
        stream_id = RewriteRuleSubtreeStream(self._adaptor, "rule id")
        stream_tokensSpec = RewriteRuleSubtreeStream(self._adaptor, "rule tokensSpec")
        stream_attrScope = RewriteRuleSubtreeStream(self._adaptor, "rule attrScope")
        stream_rule = RewriteRuleSubtreeStream(self._adaptor, "rule rule")
        stream_action = RewriteRuleSubtreeStream(self._adaptor, "rule action")
        stream_optionsSpec = RewriteRuleSubtreeStream(self._adaptor, "rule optionsSpec")
        try:
            try:
                # /home/res/ANTLRv3_py.g:88:5: ( ( DOC_COMMENT )? ( 'lexer' | 'parser' | 'tree' | ) g= 'grammar' id ';' ( optionsSpec )? ( tokensSpec )? ( attrScope )* ( action )* ( rule )+ EOF -> ^( id ( DOC_COMMENT )? ( optionsSpec )? ( tokensSpec )? ( attrScope )* ( action )* ( rule )+ ) )
                # /home/res/ANTLRv3_py.g:88:9: ( DOC_COMMENT )? ( 'lexer' | 'parser' | 'tree' | ) g= 'grammar' id ';' ( optionsSpec )? ( tokensSpec )? ( attrScope )* ( action )* ( rule )+ EOF
                pass 
                # /home/res/ANTLRv3_py.g:88:9: ( DOC_COMMENT )?
                alt1 = 2
                LA1_0 = self.input.LA(1)

                if (LA1_0 == DOC_COMMENT) :
                    alt1 = 1
                if alt1 == 1:
                    # /home/res/ANTLRv3_py.g:88:9: DOC_COMMENT
                    pass 
                    DOC_COMMENT1=self.match(self.input, DOC_COMMENT, self.FOLLOW_DOC_COMMENT_in_grammarDef341) 
                    if self._state.backtracking == 0:
                        stream_DOC_COMMENT.add(DOC_COMMENT1)



                # /home/res/ANTLRv3_py.g:89:6: ( 'lexer' | 'parser' | 'tree' | )
                alt2 = 4
                LA2 = self.input.LA(1)
                if LA2 == 65:
                    alt2 = 1
                elif LA2 == 66:
                    alt2 = 2
                elif LA2 == 67:
                    alt2 = 3
                elif LA2 == 68:
                    alt2 = 4
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed

                    nvae = NoViableAltException("", 2, 0, self.input)

                    raise nvae

                if alt2 == 1:
                    # /home/res/ANTLRv3_py.g:89:8: 'lexer'
                    pass 
                    string_literal2=self.match(self.input, 65, self.FOLLOW_65_in_grammarDef351) 
                    if self._state.backtracking == 0:
                        stream_65.add(string_literal2)
                    if self._state.backtracking == 0:
                        self.gtype=LEXER_GRAMMAR;



                elif alt2 == 2:
                    # /home/res/ANTLRv3_py.g:90:10: 'parser'
                    pass 
                    string_literal3=self.match(self.input, 66, self.FOLLOW_66_in_grammarDef369) 
                    if self._state.backtracking == 0:
                        stream_66.add(string_literal3)
                    if self._state.backtracking == 0:
                        self.gtype=PARSER_GRAMMAR;



                elif alt2 == 3:
                    # /home/res/ANTLRv3_py.g:91:10: 'tree'
                    pass 
                    string_literal4=self.match(self.input, 67, self.FOLLOW_67_in_grammarDef385) 
                    if self._state.backtracking == 0:
                        stream_67.add(string_literal4)
                    if self._state.backtracking == 0:
                        self.gtype=TREE_GRAMMAR;



                elif alt2 == 4:
                    # /home/res/ANTLRv3_py.g:92:14: 
                    pass 
                    if self._state.backtracking == 0:
                        self.gtype=COMBINED_GRAMMAR;




                g=self.match(self.input, 68, self.FOLLOW_68_in_grammarDef426) 
                if self._state.backtracking == 0:
                    stream_68.add(g)
                self._state.following.append(self.FOLLOW_id_in_grammarDef428)
                id5 = self.id()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_id.add(id5.tree)
                char_literal6=self.match(self.input, 69, self.FOLLOW_69_in_grammarDef430) 
                if self._state.backtracking == 0:
                    stream_69.add(char_literal6)
                # /home/res/ANTLRv3_py.g:94:25: ( optionsSpec )?
                alt3 = 2
                LA3_0 = self.input.LA(1)

                if (LA3_0 == OPTIONS) :
                    alt3 = 1
                if alt3 == 1:
                    # /home/res/ANTLRv3_py.g:94:25: optionsSpec
                    pass 
                    self._state.following.append(self.FOLLOW_optionsSpec_in_grammarDef432)
                    optionsSpec7 = self.optionsSpec()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_optionsSpec.add(optionsSpec7.tree)



                # /home/res/ANTLRv3_py.g:94:38: ( tokensSpec )?
                alt4 = 2
                LA4_0 = self.input.LA(1)

                if (LA4_0 == TOKENS) :
                    alt4 = 1
                if alt4 == 1:
                    # /home/res/ANTLRv3_py.g:94:38: tokensSpec
                    pass 
                    self._state.following.append(self.FOLLOW_tokensSpec_in_grammarDef435)
                    tokensSpec8 = self.tokensSpec()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_tokensSpec.add(tokensSpec8.tree)



                # /home/res/ANTLRv3_py.g:94:50: ( attrScope )*
                while True: #loop5
                    alt5 = 2
                    LA5_0 = self.input.LA(1)

                    if (LA5_0 == SCOPE) :
                        alt5 = 1


                    if alt5 == 1:
                        # /home/res/ANTLRv3_py.g:94:50: attrScope
                        pass 
                        self._state.following.append(self.FOLLOW_attrScope_in_grammarDef438)
                        attrScope9 = self.attrScope()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_attrScope.add(attrScope9.tree)


                    else:
                        break #loop5
                # /home/res/ANTLRv3_py.g:94:61: ( action )*
                while True: #loop6
                    alt6 = 2
                    LA6_0 = self.input.LA(1)

                    if (LA6_0 == 72) :
                        alt6 = 1


                    if alt6 == 1:
                        # /home/res/ANTLRv3_py.g:94:61: action
                        pass 
                        self._state.following.append(self.FOLLOW_action_in_grammarDef441)
                        action10 = self.action()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_action.add(action10.tree)


                    else:
                        break #loop6
                # /home/res/ANTLRv3_py.g:95:6: ( rule )+
                cnt7 = 0
                while True: #loop7
                    alt7 = 2
                    LA7_0 = self.input.LA(1)

                    if (LA7_0 == DOC_COMMENT or LA7_0 == FRAGMENT or LA7_0 == TOKEN_REF or LA7_0 == RULE_REF or (75 <= LA7_0 <= 77)) :
                        alt7 = 1


                    if alt7 == 1:
                        # /home/res/ANTLRv3_py.g:95:6: rule
                        pass 
                        self._state.following.append(self.FOLLOW_rule_in_grammarDef449)
                        rule11 = self.rule()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_rule.add(rule11.tree)


                    else:
                        if cnt7 >= 1:
                            break #loop7

                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        eee = EarlyExitException(7, self.input)
                        raise eee

                    cnt7 += 1
                EOF12=self.match(self.input, EOF, self.FOLLOW_EOF_in_grammarDef457) 
                if self._state.backtracking == 0:
                    stream_EOF.add(EOF12)

                # AST Rewrite
                # elements: id, action, attrScope, optionsSpec, tokensSpec, rule, DOC_COMMENT
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 97:6: -> ^( id ( DOC_COMMENT )? ( optionsSpec )? ( tokensSpec )? ( attrScope )* ( action )* ( rule )+ )
                    # /home/res/ANTLRv3_py.g:97:9: ^( id ( DOC_COMMENT )? ( optionsSpec )? ( tokensSpec )? ( attrScope )* ( action )* ( rule )+ )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(self.adaptor.create(self.gtype,g), root_1)

                    self._adaptor.addChild(root_1, stream_id.nextTree())
                    # /home/res/ANTLRv3_py.g:98:12: ( DOC_COMMENT )?
                    if stream_DOC_COMMENT.hasNext():
                        self._adaptor.addChild(root_1, stream_DOC_COMMENT.nextNode())


                    stream_DOC_COMMENT.reset();
                    # /home/res/ANTLRv3_py.g:98:25: ( optionsSpec )?
                    if stream_optionsSpec.hasNext():
                        self._adaptor.addChild(root_1, stream_optionsSpec.nextTree())


                    stream_optionsSpec.reset();
                    # /home/res/ANTLRv3_py.g:98:38: ( tokensSpec )?
                    if stream_tokensSpec.hasNext():
                        self._adaptor.addChild(root_1, stream_tokensSpec.nextTree())


                    stream_tokensSpec.reset();
                    # /home/res/ANTLRv3_py.g:98:50: ( attrScope )*
                    while stream_attrScope.hasNext():
                        self._adaptor.addChild(root_1, stream_attrScope.nextTree())


                    stream_attrScope.reset();
                    # /home/res/ANTLRv3_py.g:98:61: ( action )*
                    while stream_action.hasNext():
                        self._adaptor.addChild(root_1, stream_action.nextTree())


                    stream_action.reset();
                    # /home/res/ANTLRv3_py.g:98:69: ( rule )+
                    if not (stream_rule.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_rule.hasNext():
                        self._adaptor.addChild(root_1, stream_rule.nextTree())


                    stream_rule.reset()

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "grammarDef"

    class tokensSpec_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.tokensSpec_return, self).__init__()

            self.tree = None




    # $ANTLR start "tokensSpec"
    # /home/res/ANTLRv3_py.g:102:1: tokensSpec : TOKENS ( tokenSpec )+ '}' -> ^( TOKENS ( tokenSpec )+ ) ;
    def tokensSpec(self, ):

        retval = self.tokensSpec_return()
        retval.start = self.input.LT(1)

        root_0 = None

        TOKENS13 = None
        char_literal15 = None
        tokenSpec14 = None


        TOKENS13_tree = None
        char_literal15_tree = None
        stream_TOKENS = RewriteRuleTokenStream(self._adaptor, "token TOKENS")
        stream_70 = RewriteRuleTokenStream(self._adaptor, "token 70")
        stream_tokenSpec = RewriteRuleSubtreeStream(self._adaptor, "rule tokenSpec")
        try:
            try:
                # /home/res/ANTLRv3_py.g:103:2: ( TOKENS ( tokenSpec )+ '}' -> ^( TOKENS ( tokenSpec )+ ) )
                # /home/res/ANTLRv3_py.g:103:4: TOKENS ( tokenSpec )+ '}'
                pass 
                TOKENS13=self.match(self.input, TOKENS, self.FOLLOW_TOKENS_in_tokensSpec518) 
                if self._state.backtracking == 0:
                    stream_TOKENS.add(TOKENS13)
                # /home/res/ANTLRv3_py.g:103:11: ( tokenSpec )+
                cnt8 = 0
                while True: #loop8
                    alt8 = 2
                    LA8_0 = self.input.LA(1)

                    if (LA8_0 == TOKEN_REF) :
                        alt8 = 1


                    if alt8 == 1:
                        # /home/res/ANTLRv3_py.g:103:11: tokenSpec
                        pass 
                        self._state.following.append(self.FOLLOW_tokenSpec_in_tokensSpec520)
                        tokenSpec14 = self.tokenSpec()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_tokenSpec.add(tokenSpec14.tree)


                    else:
                        if cnt8 >= 1:
                            break #loop8

                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        eee = EarlyExitException(8, self.input)
                        raise eee

                    cnt8 += 1
                char_literal15=self.match(self.input, 70, self.FOLLOW_70_in_tokensSpec523) 
                if self._state.backtracking == 0:
                    stream_70.add(char_literal15)

                # AST Rewrite
                # elements: TOKENS, tokenSpec
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 103:26: -> ^( TOKENS ( tokenSpec )+ )
                    # /home/res/ANTLRv3_py.g:103:29: ^( TOKENS ( tokenSpec )+ )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(stream_TOKENS.nextNode(), root_1)

                    # /home/res/ANTLRv3_py.g:103:38: ( tokenSpec )+
                    if not (stream_tokenSpec.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_tokenSpec.hasNext():
                        self._adaptor.addChild(root_1, stream_tokenSpec.nextTree())


                    stream_tokenSpec.reset()

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "tokensSpec"

    class tokenSpec_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.tokenSpec_return, self).__init__()

            self.tree = None




    # $ANTLR start "tokenSpec"
    # /home/res/ANTLRv3_py.g:106:1: tokenSpec : TOKEN_REF ( '=' (lit= STRING_LITERAL | lit= CHAR_LITERAL ) -> ^( '=' TOKEN_REF $lit) | -> TOKEN_REF ) ';' ;
    def tokenSpec(self, ):

        retval = self.tokenSpec_return()
        retval.start = self.input.LT(1)

        root_0 = None

        lit = None
        TOKEN_REF16 = None
        char_literal17 = None
        char_literal18 = None

        lit_tree = None
        TOKEN_REF16_tree = None
        char_literal17_tree = None
        char_literal18_tree = None
        stream_69 = RewriteRuleTokenStream(self._adaptor, "token 69")
        stream_STRING_LITERAL = RewriteRuleTokenStream(self._adaptor, "token STRING_LITERAL")
        stream_CHAR_LITERAL = RewriteRuleTokenStream(self._adaptor, "token CHAR_LITERAL")
        stream_71 = RewriteRuleTokenStream(self._adaptor, "token 71")
        stream_TOKEN_REF = RewriteRuleTokenStream(self._adaptor, "token TOKEN_REF")

        try:
            try:
                # /home/res/ANTLRv3_py.g:107:2: ( TOKEN_REF ( '=' (lit= STRING_LITERAL | lit= CHAR_LITERAL ) -> ^( '=' TOKEN_REF $lit) | -> TOKEN_REF ) ';' )
                # /home/res/ANTLRv3_py.g:107:4: TOKEN_REF ( '=' (lit= STRING_LITERAL | lit= CHAR_LITERAL ) -> ^( '=' TOKEN_REF $lit) | -> TOKEN_REF ) ';'
                pass 
                TOKEN_REF16=self.match(self.input, TOKEN_REF, self.FOLLOW_TOKEN_REF_in_tokenSpec543) 
                if self._state.backtracking == 0:
                    stream_TOKEN_REF.add(TOKEN_REF16)
                # /home/res/ANTLRv3_py.g:108:3: ( '=' (lit= STRING_LITERAL | lit= CHAR_LITERAL ) -> ^( '=' TOKEN_REF $lit) | -> TOKEN_REF )
                alt10 = 2
                LA10_0 = self.input.LA(1)

                if (LA10_0 == 71) :
                    alt10 = 1
                elif (LA10_0 == 69) :
                    alt10 = 2
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed

                    nvae = NoViableAltException("", 10, 0, self.input)

                    raise nvae

                if alt10 == 1:
                    # /home/res/ANTLRv3_py.g:108:5: '=' (lit= STRING_LITERAL | lit= CHAR_LITERAL )
                    pass 
                    char_literal17=self.match(self.input, 71, self.FOLLOW_71_in_tokenSpec549) 
                    if self._state.backtracking == 0:
                        stream_71.add(char_literal17)
                    # /home/res/ANTLRv3_py.g:108:9: (lit= STRING_LITERAL | lit= CHAR_LITERAL )
                    alt9 = 2
                    LA9_0 = self.input.LA(1)

                    if (LA9_0 == STRING_LITERAL) :
                        alt9 = 1
                    elif (LA9_0 == CHAR_LITERAL) :
                        alt9 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        nvae = NoViableAltException("", 9, 0, self.input)

                        raise nvae

                    if alt9 == 1:
                        # /home/res/ANTLRv3_py.g:108:10: lit= STRING_LITERAL
                        pass 
                        lit=self.match(self.input, STRING_LITERAL, self.FOLLOW_STRING_LITERAL_in_tokenSpec554) 
                        if self._state.backtracking == 0:
                            stream_STRING_LITERAL.add(lit)


                    elif alt9 == 2:
                        # /home/res/ANTLRv3_py.g:108:29: lit= CHAR_LITERAL
                        pass 
                        lit=self.match(self.input, CHAR_LITERAL, self.FOLLOW_CHAR_LITERAL_in_tokenSpec558) 
                        if self._state.backtracking == 0:
                            stream_CHAR_LITERAL.add(lit)




                    # AST Rewrite
                    # elements: 71, TOKEN_REF, lit
                    # token labels: lit
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0
                        stream_lit = RewriteRuleTokenStream(self._adaptor, "token lit", lit)

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 108:47: -> ^( '=' TOKEN_REF $lit)
                        # /home/res/ANTLRv3_py.g:108:50: ^( '=' TOKEN_REF $lit)
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(stream_71.nextNode(), root_1)

                        self._adaptor.addChild(root_1, stream_TOKEN_REF.nextNode())
                        self._adaptor.addChild(root_1, stream_lit.nextNode())

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0


                elif alt10 == 2:
                    # /home/res/ANTLRv3_py.g:109:16: 
                    pass 
                    # AST Rewrite
                    # elements: TOKEN_REF
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 109:16: -> TOKEN_REF
                        self._adaptor.addChild(root_0, stream_TOKEN_REF.nextNode())



                        retval.tree = root_0



                char_literal18=self.match(self.input, 69, self.FOLLOW_69_in_tokenSpec597) 
                if self._state.backtracking == 0:
                    stream_69.add(char_literal18)



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "tokenSpec"

    class attrScope_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.attrScope_return, self).__init__()

            self.tree = None




    # $ANTLR start "attrScope"
    # /home/res/ANTLRv3_py.g:114:1: attrScope : 'scope' id ACTION -> ^( 'scope' id ACTION ) ;
    def attrScope(self, ):

        retval = self.attrScope_return()
        retval.start = self.input.LT(1)

        root_0 = None

        string_literal19 = None
        ACTION21 = None
        id20 = None


        string_literal19_tree = None
        ACTION21_tree = None
        stream_SCOPE = RewriteRuleTokenStream(self._adaptor, "token SCOPE")
        stream_ACTION = RewriteRuleTokenStream(self._adaptor, "token ACTION")
        stream_id = RewriteRuleSubtreeStream(self._adaptor, "rule id")
        try:
            try:
                # /home/res/ANTLRv3_py.g:115:2: ( 'scope' id ACTION -> ^( 'scope' id ACTION ) )
                # /home/res/ANTLRv3_py.g:115:4: 'scope' id ACTION
                pass 
                string_literal19=self.match(self.input, SCOPE, self.FOLLOW_SCOPE_in_attrScope608) 
                if self._state.backtracking == 0:
                    stream_SCOPE.add(string_literal19)
                self._state.following.append(self.FOLLOW_id_in_attrScope610)
                id20 = self.id()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_id.add(id20.tree)
                ACTION21=self.match(self.input, ACTION, self.FOLLOW_ACTION_in_attrScope612) 
                if self._state.backtracking == 0:
                    stream_ACTION.add(ACTION21)

                # AST Rewrite
                # elements: SCOPE, ACTION, id
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 115:22: -> ^( 'scope' id ACTION )
                    # /home/res/ANTLRv3_py.g:115:25: ^( 'scope' id ACTION )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(stream_SCOPE.nextNode(), root_1)

                    self._adaptor.addChild(root_1, stream_id.nextTree())
                    self._adaptor.addChild(root_1, stream_ACTION.nextNode())

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "attrScope"

    class action_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.action_return, self).__init__()

            self.tree = None




    # $ANTLR start "action"
    # /home/res/ANTLRv3_py.g:118:1: action : '@' ( actionScopeName '::' )? id ACTION -> ^( '@' ( actionScopeName )? id ACTION ) ;
    def action(self, ):

        retval = self.action_return()
        retval.start = self.input.LT(1)

        root_0 = None

        char_literal22 = None
        string_literal24 = None
        ACTION26 = None
        actionScopeName23 = None

        id25 = None


        char_literal22_tree = None
        string_literal24_tree = None
        ACTION26_tree = None
        stream_72 = RewriteRuleTokenStream(self._adaptor, "token 72")
        stream_ACTION = RewriteRuleTokenStream(self._adaptor, "token ACTION")
        stream_73 = RewriteRuleTokenStream(self._adaptor, "token 73")
        stream_id = RewriteRuleSubtreeStream(self._adaptor, "rule id")
        stream_actionScopeName = RewriteRuleSubtreeStream(self._adaptor, "rule actionScopeName")
        try:
            try:
                # /home/res/ANTLRv3_py.g:120:2: ( '@' ( actionScopeName '::' )? id ACTION -> ^( '@' ( actionScopeName )? id ACTION ) )
                # /home/res/ANTLRv3_py.g:120:4: '@' ( actionScopeName '::' )? id ACTION
                pass 
                char_literal22=self.match(self.input, 72, self.FOLLOW_72_in_action635) 
                if self._state.backtracking == 0:
                    stream_72.add(char_literal22)
                # /home/res/ANTLRv3_py.g:120:8: ( actionScopeName '::' )?
                alt11 = 2
                LA11 = self.input.LA(1)
                if LA11 == TOKEN_REF:
                    LA11_1 = self.input.LA(2)

                    if (LA11_1 == 73) :
                        alt11 = 1
                elif LA11 == RULE_REF:
                    LA11_2 = self.input.LA(2)

                    if (LA11_2 == 73) :
                        alt11 = 1
                elif LA11 == 65 or LA11 == 66:
                    alt11 = 1
                if alt11 == 1:
                    # /home/res/ANTLRv3_py.g:120:9: actionScopeName '::'
                    pass 
                    self._state.following.append(self.FOLLOW_actionScopeName_in_action638)
                    actionScopeName23 = self.actionScopeName()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_actionScopeName.add(actionScopeName23.tree)
                    string_literal24=self.match(self.input, 73, self.FOLLOW_73_in_action640) 
                    if self._state.backtracking == 0:
                        stream_73.add(string_literal24)



                self._state.following.append(self.FOLLOW_id_in_action644)
                id25 = self.id()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_id.add(id25.tree)
                ACTION26=self.match(self.input, ACTION, self.FOLLOW_ACTION_in_action646) 
                if self._state.backtracking == 0:
                    stream_ACTION.add(ACTION26)

                # AST Rewrite
                # elements: id, actionScopeName, 72, ACTION
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 120:42: -> ^( '@' ( actionScopeName )? id ACTION )
                    # /home/res/ANTLRv3_py.g:120:45: ^( '@' ( actionScopeName )? id ACTION )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(stream_72.nextNode(), root_1)

                    # /home/res/ANTLRv3_py.g:120:51: ( actionScopeName )?
                    if stream_actionScopeName.hasNext():
                        self._adaptor.addChild(root_1, stream_actionScopeName.nextTree())


                    stream_actionScopeName.reset();
                    self._adaptor.addChild(root_1, stream_id.nextTree())
                    self._adaptor.addChild(root_1, stream_ACTION.nextNode())

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "action"

    class actionScopeName_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.actionScopeName_return, self).__init__()

            self.tree = None




    # $ANTLR start "actionScopeName"
    # /home/res/ANTLRv3_py.g:123:1: actionScopeName : ( id | l= 'lexer' -> ID[$l] | p= 'parser' -> ID[$p] );
    def actionScopeName(self, ):

        retval = self.actionScopeName_return()
        retval.start = self.input.LT(1)

        root_0 = None

        l = None
        p = None
        id27 = None


        l_tree = None
        p_tree = None
        stream_66 = RewriteRuleTokenStream(self._adaptor, "token 66")
        stream_65 = RewriteRuleTokenStream(self._adaptor, "token 65")

        try:
            try:
                # /home/res/ANTLRv3_py.g:127:2: ( id | l= 'lexer' -> ID[$l] | p= 'parser' -> ID[$p] )
                alt12 = 3
                LA12 = self.input.LA(1)
                if LA12 == TOKEN_REF or LA12 == RULE_REF:
                    alt12 = 1
                elif LA12 == 65:
                    alt12 = 2
                elif LA12 == 66:
                    alt12 = 3
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed

                    nvae = NoViableAltException("", 12, 0, self.input)

                    raise nvae

                if alt12 == 1:
                    # /home/res/ANTLRv3_py.g:127:4: id
                    pass 
                    root_0 = self._adaptor.nil()

                    self._state.following.append(self.FOLLOW_id_in_actionScopeName672)
                    id27 = self.id()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, id27.tree)


                elif alt12 == 2:
                    # /home/res/ANTLRv3_py.g:128:4: l= 'lexer'
                    pass 
                    l=self.match(self.input, 65, self.FOLLOW_65_in_actionScopeName679) 
                    if self._state.backtracking == 0:
                        stream_65.add(l)

                    # AST Rewrite
                    # elements: 
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 128:14: -> ID[$l]
                        self._adaptor.addChild(root_0, self._adaptor.create(ID, l))



                        retval.tree = root_0


                elif alt12 == 3:
                    # /home/res/ANTLRv3_py.g:129:9: p= 'parser'
                    pass 
                    p=self.match(self.input, 66, self.FOLLOW_66_in_actionScopeName696) 
                    if self._state.backtracking == 0:
                        stream_66.add(p)

                    # AST Rewrite
                    # elements: 
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 129:20: -> ID[$p]
                        self._adaptor.addChild(root_0, self._adaptor.create(ID, p))



                        retval.tree = root_0


                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "actionScopeName"

    class optionsSpec_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.optionsSpec_return, self).__init__()

            self.tree = None




    # $ANTLR start "optionsSpec"
    # /home/res/ANTLRv3_py.g:132:1: optionsSpec : OPTIONS ( option ';' )+ '}' -> ^( OPTIONS ( option )+ ) ;
    def optionsSpec(self, ):

        retval = self.optionsSpec_return()
        retval.start = self.input.LT(1)

        root_0 = None

        OPTIONS28 = None
        char_literal30 = None
        char_literal31 = None
        option29 = None


        OPTIONS28_tree = None
        char_literal30_tree = None
        char_literal31_tree = None
        stream_69 = RewriteRuleTokenStream(self._adaptor, "token 69")
        stream_70 = RewriteRuleTokenStream(self._adaptor, "token 70")
        stream_OPTIONS = RewriteRuleTokenStream(self._adaptor, "token OPTIONS")
        stream_option = RewriteRuleSubtreeStream(self._adaptor, "rule option")
        try:
            try:
                # /home/res/ANTLRv3_py.g:133:2: ( OPTIONS ( option ';' )+ '}' -> ^( OPTIONS ( option )+ ) )
                # /home/res/ANTLRv3_py.g:133:4: OPTIONS ( option ';' )+ '}'
                pass 
                OPTIONS28=self.match(self.input, OPTIONS, self.FOLLOW_OPTIONS_in_optionsSpec712) 
                if self._state.backtracking == 0:
                    stream_OPTIONS.add(OPTIONS28)
                # /home/res/ANTLRv3_py.g:133:12: ( option ';' )+
                cnt13 = 0
                while True: #loop13
                    alt13 = 2
                    LA13_0 = self.input.LA(1)

                    if (LA13_0 == TOKEN_REF or LA13_0 == RULE_REF) :
                        alt13 = 1


                    if alt13 == 1:
                        # /home/res/ANTLRv3_py.g:133:13: option ';'
                        pass 
                        self._state.following.append(self.FOLLOW_option_in_optionsSpec715)
                        option29 = self.option()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_option.add(option29.tree)
                        char_literal30=self.match(self.input, 69, self.FOLLOW_69_in_optionsSpec717) 
                        if self._state.backtracking == 0:
                            stream_69.add(char_literal30)


                    else:
                        if cnt13 >= 1:
                            break #loop13

                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        eee = EarlyExitException(13, self.input)
                        raise eee

                    cnt13 += 1
                char_literal31=self.match(self.input, 70, self.FOLLOW_70_in_optionsSpec721) 
                if self._state.backtracking == 0:
                    stream_70.add(char_literal31)

                # AST Rewrite
                # elements: OPTIONS, option
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 133:30: -> ^( OPTIONS ( option )+ )
                    # /home/res/ANTLRv3_py.g:133:33: ^( OPTIONS ( option )+ )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(stream_OPTIONS.nextNode(), root_1)

                    # /home/res/ANTLRv3_py.g:133:43: ( option )+
                    if not (stream_option.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_option.hasNext():
                        self._adaptor.addChild(root_1, stream_option.nextTree())


                    stream_option.reset()

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "optionsSpec"

    class option_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.option_return, self).__init__()

            self.tree = None




    # $ANTLR start "option"
    # /home/res/ANTLRv3_py.g:136:1: option : id '=' optionValue -> ^( '=' id optionValue ) ;
    def option(self, ):

        retval = self.option_return()
        retval.start = self.input.LT(1)

        root_0 = None

        char_literal33 = None
        id32 = None

        optionValue34 = None


        char_literal33_tree = None
        stream_71 = RewriteRuleTokenStream(self._adaptor, "token 71")
        stream_id = RewriteRuleSubtreeStream(self._adaptor, "rule id")
        stream_optionValue = RewriteRuleSubtreeStream(self._adaptor, "rule optionValue")
        try:
            try:
                # /home/res/ANTLRv3_py.g:137:5: ( id '=' optionValue -> ^( '=' id optionValue ) )
                # /home/res/ANTLRv3_py.g:137:9: id '=' optionValue
                pass 
                self._state.following.append(self.FOLLOW_id_in_option746)
                id32 = self.id()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_id.add(id32.tree)
                char_literal33=self.match(self.input, 71, self.FOLLOW_71_in_option748) 
                if self._state.backtracking == 0:
                    stream_71.add(char_literal33)
                self._state.following.append(self.FOLLOW_optionValue_in_option750)
                optionValue34 = self.optionValue()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_optionValue.add(optionValue34.tree)

                # AST Rewrite
                # elements: optionValue, id, 71
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 137:28: -> ^( '=' id optionValue )
                    # /home/res/ANTLRv3_py.g:137:31: ^( '=' id optionValue )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(stream_71.nextNode(), root_1)

                    self._adaptor.addChild(root_1, stream_id.nextTree())
                    self._adaptor.addChild(root_1, stream_optionValue.nextTree())

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "option"

    class optionValue_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.optionValue_return, self).__init__()

            self.tree = None




    # $ANTLR start "optionValue"
    # /home/res/ANTLRv3_py.g:140:1: optionValue : ( id | STRING_LITERAL | CHAR_LITERAL | INT | s= '*' -> STRING_LITERAL[$s] );
    def optionValue(self, ):

        retval = self.optionValue_return()
        retval.start = self.input.LT(1)

        root_0 = None

        s = None
        STRING_LITERAL36 = None
        CHAR_LITERAL37 = None
        INT38 = None
        id35 = None


        s_tree = None
        STRING_LITERAL36_tree = None
        CHAR_LITERAL37_tree = None
        INT38_tree = None
        stream_74 = RewriteRuleTokenStream(self._adaptor, "token 74")

        try:
            try:
                # /home/res/ANTLRv3_py.g:141:5: ( id | STRING_LITERAL | CHAR_LITERAL | INT | s= '*' -> STRING_LITERAL[$s] )
                alt14 = 5
                LA14 = self.input.LA(1)
                if LA14 == TOKEN_REF or LA14 == RULE_REF:
                    alt14 = 1
                elif LA14 == STRING_LITERAL:
                    alt14 = 2
                elif LA14 == CHAR_LITERAL:
                    alt14 = 3
                elif LA14 == INT:
                    alt14 = 4
                elif LA14 == 74:
                    alt14 = 5
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed

                    nvae = NoViableAltException("", 14, 0, self.input)

                    raise nvae

                if alt14 == 1:
                    # /home/res/ANTLRv3_py.g:141:9: id
                    pass 
                    root_0 = self._adaptor.nil()

                    self._state.following.append(self.FOLLOW_id_in_optionValue779)
                    id35 = self.id()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, id35.tree)


                elif alt14 == 2:
                    # /home/res/ANTLRv3_py.g:142:9: STRING_LITERAL
                    pass 
                    root_0 = self._adaptor.nil()

                    STRING_LITERAL36=self.match(self.input, STRING_LITERAL, self.FOLLOW_STRING_LITERAL_in_optionValue789)
                    if self._state.backtracking == 0:

                        STRING_LITERAL36_tree = self._adaptor.createWithPayload(STRING_LITERAL36)
                        self._adaptor.addChild(root_0, STRING_LITERAL36_tree)



                elif alt14 == 3:
                    # /home/res/ANTLRv3_py.g:143:9: CHAR_LITERAL
                    pass 
                    root_0 = self._adaptor.nil()

                    CHAR_LITERAL37=self.match(self.input, CHAR_LITERAL, self.FOLLOW_CHAR_LITERAL_in_optionValue799)
                    if self._state.backtracking == 0:

                        CHAR_LITERAL37_tree = self._adaptor.createWithPayload(CHAR_LITERAL37)
                        self._adaptor.addChild(root_0, CHAR_LITERAL37_tree)



                elif alt14 == 4:
                    # /home/res/ANTLRv3_py.g:144:9: INT
                    pass 
                    root_0 = self._adaptor.nil()

                    INT38=self.match(self.input, INT, self.FOLLOW_INT_in_optionValue809)
                    if self._state.backtracking == 0:

                        INT38_tree = self._adaptor.createWithPayload(INT38)
                        self._adaptor.addChild(root_0, INT38_tree)



                elif alt14 == 5:
                    # /home/res/ANTLRv3_py.g:145:7: s= '*'
                    pass 
                    s=self.match(self.input, 74, self.FOLLOW_74_in_optionValue819) 
                    if self._state.backtracking == 0:
                        stream_74.add(s)

                    # AST Rewrite
                    # elements: 
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 145:13: -> STRING_LITERAL[$s]
                        self._adaptor.addChild(root_0, self._adaptor.create(STRING_LITERAL, s))



                        retval.tree = root_0


                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "optionValue"

    class rule_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.rule_return, self).__init__()

            self.tree = None




    # $ANTLR start "rule"
    # /home/res/ANTLRv3_py.g:148:1: rule : ( DOC_COMMENT )* (modifier= ( 'protected' | 'public' | 'private' | 'fragment' ) )? id ( '!' )? (arg= ARG_ACTION )? ( 'returns' rt= ARG_ACTION )? ( throwsSpec )? ( optionsSpec )? ( ruleScopeSpec )? ( ruleAction )* ':' altList ';' ( exceptionGroup )? -> ^( RULE id ( ^( ARG $arg) )? ( ^( RET $rt) )? ( DOC_COMMENT )* ( optionsSpec )? ( ruleScopeSpec )? ( ruleAction )* altList ( exceptionGroup )? EOR[\"EOR\"] ) ;
    def rule(self, ):
        self.rule_stack.append(rule_scope())
        retval = self.rule_return()
        retval.start = self.input.LT(1)

        root_0 = None

        modifier = None
        arg = None
        rt = None
        DOC_COMMENT39 = None
        string_literal40 = None
        string_literal41 = None
        string_literal42 = None
        string_literal43 = None
        char_literal45 = None
        string_literal46 = None
        char_literal51 = None
        char_literal53 = None
        id44 = None

        throwsSpec47 = None

        optionsSpec48 = None

        ruleScopeSpec49 = None

        ruleAction50 = None

        altList52 = None

        exceptionGroup54 = None


        modifier_tree = None
        arg_tree = None
        rt_tree = None
        DOC_COMMENT39_tree = None
        string_literal40_tree = None
        string_literal41_tree = None
        string_literal42_tree = None
        string_literal43_tree = None
        char_literal45_tree = None
        string_literal46_tree = None
        char_literal51_tree = None
        char_literal53_tree = None
        stream_DOC_COMMENT = RewriteRuleTokenStream(self._adaptor, "token DOC_COMMENT")
        stream_79 = RewriteRuleTokenStream(self._adaptor, "token 79")
        stream_78 = RewriteRuleTokenStream(self._adaptor, "token 78")
        stream_69 = RewriteRuleTokenStream(self._adaptor, "token 69")
        stream_77 = RewriteRuleTokenStream(self._adaptor, "token 77")
        stream_BANG = RewriteRuleTokenStream(self._adaptor, "token BANG")
        stream_FRAGMENT = RewriteRuleTokenStream(self._adaptor, "token FRAGMENT")
        stream_75 = RewriteRuleTokenStream(self._adaptor, "token 75")
        stream_ARG_ACTION = RewriteRuleTokenStream(self._adaptor, "token ARG_ACTION")
        stream_76 = RewriteRuleTokenStream(self._adaptor, "token 76")
        stream_id = RewriteRuleSubtreeStream(self._adaptor, "rule id")
        stream_exceptionGroup = RewriteRuleSubtreeStream(self._adaptor, "rule exceptionGroup")
        stream_throwsSpec = RewriteRuleSubtreeStream(self._adaptor, "rule throwsSpec")
        stream_ruleScopeSpec = RewriteRuleSubtreeStream(self._adaptor, "rule ruleScopeSpec")
        stream_optionsSpec = RewriteRuleSubtreeStream(self._adaptor, "rule optionsSpec")
        stream_altList = RewriteRuleSubtreeStream(self._adaptor, "rule altList")
        stream_ruleAction = RewriteRuleSubtreeStream(self._adaptor, "rule ruleAction")
        try:
            try:
                # /home/res/ANTLRv3_py.g:153:2: ( ( DOC_COMMENT )* (modifier= ( 'protected' | 'public' | 'private' | 'fragment' ) )? id ( '!' )? (arg= ARG_ACTION )? ( 'returns' rt= ARG_ACTION )? ( throwsSpec )? ( optionsSpec )? ( ruleScopeSpec )? ( ruleAction )* ':' altList ';' ( exceptionGroup )? -> ^( RULE id ( ^( ARG $arg) )? ( ^( RET $rt) )? ( DOC_COMMENT )* ( optionsSpec )? ( ruleScopeSpec )? ( ruleAction )* altList ( exceptionGroup )? EOR[\"EOR\"] ) )
                # /home/res/ANTLRv3_py.g:153:4: ( DOC_COMMENT )* (modifier= ( 'protected' | 'public' | 'private' | 'fragment' ) )? id ( '!' )? (arg= ARG_ACTION )? ( 'returns' rt= ARG_ACTION )? ( throwsSpec )? ( optionsSpec )? ( ruleScopeSpec )? ( ruleAction )* ':' altList ';' ( exceptionGroup )?
                pass 
                # /home/res/ANTLRv3_py.g:153:4: ( DOC_COMMENT )*
                while True: #loop15
                    alt15 = 2
                    LA15_0 = self.input.LA(1)

                    if (LA15_0 == DOC_COMMENT) :
                        alt15 = 1


                    if alt15 == 1:
                        # /home/res/ANTLRv3_py.g:153:4: DOC_COMMENT
                        pass 
                        DOC_COMMENT39=self.match(self.input, DOC_COMMENT, self.FOLLOW_DOC_COMMENT_in_rule844) 
                        if self._state.backtracking == 0:
                            stream_DOC_COMMENT.add(DOC_COMMENT39)


                    else:
                        break #loop15
                # /home/res/ANTLRv3_py.g:154:3: (modifier= ( 'protected' | 'public' | 'private' | 'fragment' ) )?
                alt17 = 2
                LA17_0 = self.input.LA(1)

                if (LA17_0 == FRAGMENT or (75 <= LA17_0 <= 77)) :
                    alt17 = 1
                if alt17 == 1:
                    # /home/res/ANTLRv3_py.g:154:5: modifier= ( 'protected' | 'public' | 'private' | 'fragment' )
                    pass 
                    # /home/res/ANTLRv3_py.g:154:14: ( 'protected' | 'public' | 'private' | 'fragment' )
                    alt16 = 4
                    LA16 = self.input.LA(1)
                    if LA16 == 75:
                        alt16 = 1
                    elif LA16 == 76:
                        alt16 = 2
                    elif LA16 == 77:
                        alt16 = 3
                    elif LA16 == FRAGMENT:
                        alt16 = 4
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        nvae = NoViableAltException("", 16, 0, self.input)

                        raise nvae

                    if alt16 == 1:
                        # /home/res/ANTLRv3_py.g:154:15: 'protected'
                        pass 
                        string_literal40=self.match(self.input, 75, self.FOLLOW_75_in_rule854) 
                        if self._state.backtracking == 0:
                            stream_75.add(string_literal40)


                    elif alt16 == 2:
                        # /home/res/ANTLRv3_py.g:154:27: 'public'
                        pass 
                        string_literal41=self.match(self.input, 76, self.FOLLOW_76_in_rule856) 
                        if self._state.backtracking == 0:
                            stream_76.add(string_literal41)


                    elif alt16 == 3:
                        # /home/res/ANTLRv3_py.g:154:36: 'private'
                        pass 
                        string_literal42=self.match(self.input, 77, self.FOLLOW_77_in_rule858) 
                        if self._state.backtracking == 0:
                            stream_77.add(string_literal42)


                    elif alt16 == 4:
                        # /home/res/ANTLRv3_py.g:154:46: 'fragment'
                        pass 
                        string_literal43=self.match(self.input, FRAGMENT, self.FOLLOW_FRAGMENT_in_rule860) 
                        if self._state.backtracking == 0:
                            stream_FRAGMENT.add(string_literal43)






                self._state.following.append(self.FOLLOW_id_in_rule868)
                id44 = self.id()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_id.add(id44.tree)
                if self._state.backtracking == 0:
                    self.rule_stack[-1].name =  ((id44 is not None) and [self.input.toString(id44.start,id44.stop)] or [None])[0]

                # /home/res/ANTLRv3_py.g:156:3: ( '!' )?
                alt18 = 2
                LA18_0 = self.input.LA(1)

                if (LA18_0 == BANG) :
                    alt18 = 1
                if alt18 == 1:
                    # /home/res/ANTLRv3_py.g:156:3: '!'
                    pass 
                    char_literal45=self.match(self.input, BANG, self.FOLLOW_BANG_in_rule874) 
                    if self._state.backtracking == 0:
                        stream_BANG.add(char_literal45)



                # /home/res/ANTLRv3_py.g:157:3: (arg= ARG_ACTION )?
                alt19 = 2
                LA19_0 = self.input.LA(1)

                if (LA19_0 == ARG_ACTION) :
                    alt19 = 1
                if alt19 == 1:
                    # /home/res/ANTLRv3_py.g:157:5: arg= ARG_ACTION
                    pass 
                    arg=self.match(self.input, ARG_ACTION, self.FOLLOW_ARG_ACTION_in_rule883) 
                    if self._state.backtracking == 0:
                        stream_ARG_ACTION.add(arg)



                # /home/res/ANTLRv3_py.g:158:3: ( 'returns' rt= ARG_ACTION )?
                alt20 = 2
                LA20_0 = self.input.LA(1)

                if (LA20_0 == 78) :
                    alt20 = 1
                if alt20 == 1:
                    # /home/res/ANTLRv3_py.g:158:5: 'returns' rt= ARG_ACTION
                    pass 
                    string_literal46=self.match(self.input, 78, self.FOLLOW_78_in_rule892) 
                    if self._state.backtracking == 0:
                        stream_78.add(string_literal46)
                    rt=self.match(self.input, ARG_ACTION, self.FOLLOW_ARG_ACTION_in_rule896) 
                    if self._state.backtracking == 0:
                        stream_ARG_ACTION.add(rt)



                # /home/res/ANTLRv3_py.g:159:3: ( throwsSpec )?
                alt21 = 2
                LA21_0 = self.input.LA(1)

                if (LA21_0 == 80) :
                    alt21 = 1
                if alt21 == 1:
                    # /home/res/ANTLRv3_py.g:159:3: throwsSpec
                    pass 
                    self._state.following.append(self.FOLLOW_throwsSpec_in_rule904)
                    throwsSpec47 = self.throwsSpec()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_throwsSpec.add(throwsSpec47.tree)



                # /home/res/ANTLRv3_py.g:159:15: ( optionsSpec )?
                alt22 = 2
                LA22_0 = self.input.LA(1)

                if (LA22_0 == OPTIONS) :
                    alt22 = 1
                if alt22 == 1:
                    # /home/res/ANTLRv3_py.g:159:15: optionsSpec
                    pass 
                    self._state.following.append(self.FOLLOW_optionsSpec_in_rule907)
                    optionsSpec48 = self.optionsSpec()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_optionsSpec.add(optionsSpec48.tree)



                # /home/res/ANTLRv3_py.g:159:28: ( ruleScopeSpec )?
                alt23 = 2
                LA23_0 = self.input.LA(1)

                if (LA23_0 == SCOPE) :
                    alt23 = 1
                if alt23 == 1:
                    # /home/res/ANTLRv3_py.g:159:28: ruleScopeSpec
                    pass 
                    self._state.following.append(self.FOLLOW_ruleScopeSpec_in_rule910)
                    ruleScopeSpec49 = self.ruleScopeSpec()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_ruleScopeSpec.add(ruleScopeSpec49.tree)



                # /home/res/ANTLRv3_py.g:159:43: ( ruleAction )*
                while True: #loop24
                    alt24 = 2
                    LA24_0 = self.input.LA(1)

                    if (LA24_0 == 72) :
                        alt24 = 1


                    if alt24 == 1:
                        # /home/res/ANTLRv3_py.g:159:43: ruleAction
                        pass 
                        self._state.following.append(self.FOLLOW_ruleAction_in_rule913)
                        ruleAction50 = self.ruleAction()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_ruleAction.add(ruleAction50.tree)


                    else:
                        break #loop24
                char_literal51=self.match(self.input, 79, self.FOLLOW_79_in_rule918) 
                if self._state.backtracking == 0:
                    stream_79.add(char_literal51)
                self._state.following.append(self.FOLLOW_altList_in_rule920)
                altList52 = self.altList()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_altList.add(altList52.tree)
                char_literal53=self.match(self.input, 69, self.FOLLOW_69_in_rule922) 
                if self._state.backtracking == 0:
                    stream_69.add(char_literal53)
                # /home/res/ANTLRv3_py.g:161:3: ( exceptionGroup )?
                alt25 = 2
                LA25_0 = self.input.LA(1)

                if ((85 <= LA25_0 <= 86)) :
                    alt25 = 1
                if alt25 == 1:
                    # /home/res/ANTLRv3_py.g:161:3: exceptionGroup
                    pass 
                    self._state.following.append(self.FOLLOW_exceptionGroup_in_rule926)
                    exceptionGroup54 = self.exceptionGroup()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_exceptionGroup.add(exceptionGroup54.tree)




                # AST Rewrite
                # elements: optionsSpec, exceptionGroup, ruleScopeSpec, DOC_COMMENT, arg, ruleAction, id, altList, rt
                # token labels: arg, rt
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0
                    stream_arg = RewriteRuleTokenStream(self._adaptor, "token arg", arg)
                    stream_rt = RewriteRuleTokenStream(self._adaptor, "token rt", rt)

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 162:6: -> ^( RULE id ( ^( ARG $arg) )? ( ^( RET $rt) )? ( DOC_COMMENT )* ( optionsSpec )? ( ruleScopeSpec )? ( ruleAction )* altList ( exceptionGroup )? EOR[\"EOR\"] )
                    # /home/res/ANTLRv3_py.g:162:9: ^( RULE id ( ^( ARG $arg) )? ( ^( RET $rt) )? ( DOC_COMMENT )* ( optionsSpec )? ( ruleScopeSpec )? ( ruleAction )* altList ( exceptionGroup )? EOR[\"EOR\"] )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(self._adaptor.createFromType(RULE, "RULE"), root_1)

                    self._adaptor.addChild(root_1, stream_id.nextTree())
                    self._adaptor.addChild(root_1, adaptor.create(modifier) if modifier else None)
                    # /home/res/ANTLRv3_py.g:162:69: ( ^( ARG $arg) )?
                    if stream_arg.hasNext():
                        # /home/res/ANTLRv3_py.g:162:69: ^( ARG $arg)
                        root_2 = self._adaptor.nil()
                        root_2 = self._adaptor.becomeRoot(self._adaptor.createFromType(ARG, "ARG"), root_2)

                        self._adaptor.addChild(root_2, stream_arg.nextNode())

                        self._adaptor.addChild(root_1, root_2)


                    stream_arg.reset();
                    # /home/res/ANTLRv3_py.g:162:82: ( ^( RET $rt) )?
                    if stream_rt.hasNext():
                        # /home/res/ANTLRv3_py.g:162:82: ^( RET $rt)
                        root_2 = self._adaptor.nil()
                        root_2 = self._adaptor.becomeRoot(self._adaptor.createFromType(RET, "RET"), root_2)

                        self._adaptor.addChild(root_2, stream_rt.nextNode())

                        self._adaptor.addChild(root_1, root_2)


                    stream_rt.reset();
                    # /home/res/ANTLRv3_py.g:163:9: ( DOC_COMMENT )*
                    while stream_DOC_COMMENT.hasNext():
                        self._adaptor.addChild(root_1, stream_DOC_COMMENT.nextNode())


                    stream_DOC_COMMENT.reset();
                    # /home/res/ANTLRv3_py.g:164:9: ( optionsSpec )?
                    if stream_optionsSpec.hasNext():
                        self._adaptor.addChild(root_1, stream_optionsSpec.nextTree())


                    stream_optionsSpec.reset();
                    # /home/res/ANTLRv3_py.g:164:22: ( ruleScopeSpec )?
                    if stream_ruleScopeSpec.hasNext():
                        self._adaptor.addChild(root_1, stream_ruleScopeSpec.nextTree())


                    stream_ruleScopeSpec.reset();
                    # /home/res/ANTLRv3_py.g:164:37: ( ruleAction )*
                    while stream_ruleAction.hasNext():
                        self._adaptor.addChild(root_1, stream_ruleAction.nextTree())


                    stream_ruleAction.reset();
                    self._adaptor.addChild(root_1, stream_altList.nextTree())
                    # /home/res/ANTLRv3_py.g:166:9: ( exceptionGroup )?
                    if stream_exceptionGroup.hasNext():
                        self._adaptor.addChild(root_1, stream_exceptionGroup.nextTree())


                    stream_exceptionGroup.reset();
                    self._adaptor.addChild(root_1, self._adaptor.create(EOR, "EOR"))

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            self.rule_stack.pop()
            pass
        return retval

    # $ANTLR end "rule"

    class ruleAction_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.ruleAction_return, self).__init__()

            self.tree = None




    # $ANTLR start "ruleAction"
    # /home/res/ANTLRv3_py.g:171:1: ruleAction : '@' id ACTION -> ^( '@' id ACTION ) ;
    def ruleAction(self, ):

        retval = self.ruleAction_return()
        retval.start = self.input.LT(1)

        root_0 = None

        char_literal55 = None
        ACTION57 = None
        id56 = None


        char_literal55_tree = None
        ACTION57_tree = None
        stream_72 = RewriteRuleTokenStream(self._adaptor, "token 72")
        stream_ACTION = RewriteRuleTokenStream(self._adaptor, "token ACTION")
        stream_id = RewriteRuleSubtreeStream(self._adaptor, "rule id")
        try:
            try:
                # /home/res/ANTLRv3_py.g:173:2: ( '@' id ACTION -> ^( '@' id ACTION ) )
                # /home/res/ANTLRv3_py.g:173:4: '@' id ACTION
                pass 
                char_literal55=self.match(self.input, 72, self.FOLLOW_72_in_ruleAction1039) 
                if self._state.backtracking == 0:
                    stream_72.add(char_literal55)
                self._state.following.append(self.FOLLOW_id_in_ruleAction1041)
                id56 = self.id()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_id.add(id56.tree)
                ACTION57=self.match(self.input, ACTION, self.FOLLOW_ACTION_in_ruleAction1043) 
                if self._state.backtracking == 0:
                    stream_ACTION.add(ACTION57)

                # AST Rewrite
                # elements: 72, id, ACTION
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 173:18: -> ^( '@' id ACTION )
                    # /home/res/ANTLRv3_py.g:173:21: ^( '@' id ACTION )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(stream_72.nextNode(), root_1)

                    self._adaptor.addChild(root_1, stream_id.nextTree())
                    self._adaptor.addChild(root_1, stream_ACTION.nextNode())

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "ruleAction"

    class throwsSpec_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.throwsSpec_return, self).__init__()

            self.tree = None




    # $ANTLR start "throwsSpec"
    # /home/res/ANTLRv3_py.g:176:1: throwsSpec : 'throws' id ( ',' id )* -> ^( 'throws' ( id )+ ) ;
    def throwsSpec(self, ):

        retval = self.throwsSpec_return()
        retval.start = self.input.LT(1)

        root_0 = None

        string_literal58 = None
        char_literal60 = None
        id59 = None

        id61 = None


        string_literal58_tree = None
        char_literal60_tree = None
        stream_80 = RewriteRuleTokenStream(self._adaptor, "token 80")
        stream_81 = RewriteRuleTokenStream(self._adaptor, "token 81")
        stream_id = RewriteRuleSubtreeStream(self._adaptor, "rule id")
        try:
            try:
                # /home/res/ANTLRv3_py.g:177:2: ( 'throws' id ( ',' id )* -> ^( 'throws' ( id )+ ) )
                # /home/res/ANTLRv3_py.g:177:4: 'throws' id ( ',' id )*
                pass 
                string_literal58=self.match(self.input, 80, self.FOLLOW_80_in_throwsSpec1064) 
                if self._state.backtracking == 0:
                    stream_80.add(string_literal58)
                self._state.following.append(self.FOLLOW_id_in_throwsSpec1066)
                id59 = self.id()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_id.add(id59.tree)
                # /home/res/ANTLRv3_py.g:177:16: ( ',' id )*
                while True: #loop26
                    alt26 = 2
                    LA26_0 = self.input.LA(1)

                    if (LA26_0 == 81) :
                        alt26 = 1


                    if alt26 == 1:
                        # /home/res/ANTLRv3_py.g:177:18: ',' id
                        pass 
                        char_literal60=self.match(self.input, 81, self.FOLLOW_81_in_throwsSpec1070) 
                        if self._state.backtracking == 0:
                            stream_81.add(char_literal60)
                        self._state.following.append(self.FOLLOW_id_in_throwsSpec1072)
                        id61 = self.id()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_id.add(id61.tree)


                    else:
                        break #loop26

                # AST Rewrite
                # elements: 80, id
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 177:28: -> ^( 'throws' ( id )+ )
                    # /home/res/ANTLRv3_py.g:177:31: ^( 'throws' ( id )+ )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(stream_80.nextNode(), root_1)

                    # /home/res/ANTLRv3_py.g:177:42: ( id )+
                    if not (stream_id.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_id.hasNext():
                        self._adaptor.addChild(root_1, stream_id.nextTree())


                    stream_id.reset()

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "throwsSpec"

    class ruleScopeSpec_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.ruleScopeSpec_return, self).__init__()

            self.tree = None




    # $ANTLR start "ruleScopeSpec"
    # /home/res/ANTLRv3_py.g:180:1: ruleScopeSpec : ( 'scope' ACTION -> ^( 'scope' ACTION ) | 'scope' id ( ',' id )* ';' -> ^( 'scope' ( id )+ ) | 'scope' ACTION 'scope' id ( ',' id )* ';' -> ^( 'scope' ACTION ( id )+ ) );
    def ruleScopeSpec(self, ):

        retval = self.ruleScopeSpec_return()
        retval.start = self.input.LT(1)

        root_0 = None

        string_literal62 = None
        ACTION63 = None
        string_literal64 = None
        char_literal66 = None
        char_literal68 = None
        string_literal69 = None
        ACTION70 = None
        string_literal71 = None
        char_literal73 = None
        char_literal75 = None
        id65 = None

        id67 = None

        id72 = None

        id74 = None


        string_literal62_tree = None
        ACTION63_tree = None
        string_literal64_tree = None
        char_literal66_tree = None
        char_literal68_tree = None
        string_literal69_tree = None
        ACTION70_tree = None
        string_literal71_tree = None
        char_literal73_tree = None
        char_literal75_tree = None
        stream_69 = RewriteRuleTokenStream(self._adaptor, "token 69")
        stream_SCOPE = RewriteRuleTokenStream(self._adaptor, "token SCOPE")
        stream_81 = RewriteRuleTokenStream(self._adaptor, "token 81")
        stream_ACTION = RewriteRuleTokenStream(self._adaptor, "token ACTION")
        stream_id = RewriteRuleSubtreeStream(self._adaptor, "rule id")
        try:
            try:
                # /home/res/ANTLRv3_py.g:181:2: ( 'scope' ACTION -> ^( 'scope' ACTION ) | 'scope' id ( ',' id )* ';' -> ^( 'scope' ( id )+ ) | 'scope' ACTION 'scope' id ( ',' id )* ';' -> ^( 'scope' ACTION ( id )+ ) )
                alt29 = 3
                LA29_0 = self.input.LA(1)

                if (LA29_0 == SCOPE) :
                    LA29_1 = self.input.LA(2)

                    if (LA29_1 == ACTION) :
                        LA29_2 = self.input.LA(3)

                        if (LA29_2 == SCOPE) :
                            alt29 = 3
                        elif (LA29_2 == 72 or LA29_2 == 79) :
                            alt29 = 1
                        else:
                            if self._state.backtracking > 0:
                                raise BacktrackingFailed

                            nvae = NoViableAltException("", 29, 2, self.input)

                            raise nvae

                    elif (LA29_1 == TOKEN_REF or LA29_1 == RULE_REF) :
                        alt29 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        nvae = NoViableAltException("", 29, 1, self.input)

                        raise nvae

                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed

                    nvae = NoViableAltException("", 29, 0, self.input)

                    raise nvae

                if alt29 == 1:
                    # /home/res/ANTLRv3_py.g:181:4: 'scope' ACTION
                    pass 
                    string_literal62=self.match(self.input, SCOPE, self.FOLLOW_SCOPE_in_ruleScopeSpec1095) 
                    if self._state.backtracking == 0:
                        stream_SCOPE.add(string_literal62)
                    ACTION63=self.match(self.input, ACTION, self.FOLLOW_ACTION_in_ruleScopeSpec1097) 
                    if self._state.backtracking == 0:
                        stream_ACTION.add(ACTION63)

                    # AST Rewrite
                    # elements: SCOPE, ACTION
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 181:19: -> ^( 'scope' ACTION )
                        # /home/res/ANTLRv3_py.g:181:22: ^( 'scope' ACTION )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(stream_SCOPE.nextNode(), root_1)

                        self._adaptor.addChild(root_1, stream_ACTION.nextNode())

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0


                elif alt29 == 2:
                    # /home/res/ANTLRv3_py.g:182:4: 'scope' id ( ',' id )* ';'
                    pass 
                    string_literal64=self.match(self.input, SCOPE, self.FOLLOW_SCOPE_in_ruleScopeSpec1110) 
                    if self._state.backtracking == 0:
                        stream_SCOPE.add(string_literal64)
                    self._state.following.append(self.FOLLOW_id_in_ruleScopeSpec1112)
                    id65 = self.id()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_id.add(id65.tree)
                    # /home/res/ANTLRv3_py.g:182:15: ( ',' id )*
                    while True: #loop27
                        alt27 = 2
                        LA27_0 = self.input.LA(1)

                        if (LA27_0 == 81) :
                            alt27 = 1


                        if alt27 == 1:
                            # /home/res/ANTLRv3_py.g:182:16: ',' id
                            pass 
                            char_literal66=self.match(self.input, 81, self.FOLLOW_81_in_ruleScopeSpec1115) 
                            if self._state.backtracking == 0:
                                stream_81.add(char_literal66)
                            self._state.following.append(self.FOLLOW_id_in_ruleScopeSpec1117)
                            id67 = self.id()

                            self._state.following.pop()
                            if self._state.backtracking == 0:
                                stream_id.add(id67.tree)


                        else:
                            break #loop27
                    char_literal68=self.match(self.input, 69, self.FOLLOW_69_in_ruleScopeSpec1121) 
                    if self._state.backtracking == 0:
                        stream_69.add(char_literal68)

                    # AST Rewrite
                    # elements: id, SCOPE
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 182:29: -> ^( 'scope' ( id )+ )
                        # /home/res/ANTLRv3_py.g:182:32: ^( 'scope' ( id )+ )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(stream_SCOPE.nextNode(), root_1)

                        # /home/res/ANTLRv3_py.g:182:42: ( id )+
                        if not (stream_id.hasNext()):
                            raise RewriteEarlyExitException()

                        while stream_id.hasNext():
                            self._adaptor.addChild(root_1, stream_id.nextTree())


                        stream_id.reset()

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0


                elif alt29 == 3:
                    # /home/res/ANTLRv3_py.g:183:4: 'scope' ACTION 'scope' id ( ',' id )* ';'
                    pass 
                    string_literal69=self.match(self.input, SCOPE, self.FOLLOW_SCOPE_in_ruleScopeSpec1135) 
                    if self._state.backtracking == 0:
                        stream_SCOPE.add(string_literal69)
                    ACTION70=self.match(self.input, ACTION, self.FOLLOW_ACTION_in_ruleScopeSpec1137) 
                    if self._state.backtracking == 0:
                        stream_ACTION.add(ACTION70)
                    string_literal71=self.match(self.input, SCOPE, self.FOLLOW_SCOPE_in_ruleScopeSpec1141) 
                    if self._state.backtracking == 0:
                        stream_SCOPE.add(string_literal71)
                    self._state.following.append(self.FOLLOW_id_in_ruleScopeSpec1143)
                    id72 = self.id()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_id.add(id72.tree)
                    # /home/res/ANTLRv3_py.g:184:14: ( ',' id )*
                    while True: #loop28
                        alt28 = 2
                        LA28_0 = self.input.LA(1)

                        if (LA28_0 == 81) :
                            alt28 = 1


                        if alt28 == 1:
                            # /home/res/ANTLRv3_py.g:184:15: ',' id
                            pass 
                            char_literal73=self.match(self.input, 81, self.FOLLOW_81_in_ruleScopeSpec1146) 
                            if self._state.backtracking == 0:
                                stream_81.add(char_literal73)
                            self._state.following.append(self.FOLLOW_id_in_ruleScopeSpec1148)
                            id74 = self.id()

                            self._state.following.pop()
                            if self._state.backtracking == 0:
                                stream_id.add(id74.tree)


                        else:
                            break #loop28
                    char_literal75=self.match(self.input, 69, self.FOLLOW_69_in_ruleScopeSpec1152) 
                    if self._state.backtracking == 0:
                        stream_69.add(char_literal75)

                    # AST Rewrite
                    # elements: SCOPE, id, ACTION
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 185:3: -> ^( 'scope' ACTION ( id )+ )
                        # /home/res/ANTLRv3_py.g:185:6: ^( 'scope' ACTION ( id )+ )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(stream_SCOPE.nextNode(), root_1)

                        self._adaptor.addChild(root_1, stream_ACTION.nextNode())
                        # /home/res/ANTLRv3_py.g:185:23: ( id )+
                        if not (stream_id.hasNext()):
                            raise RewriteEarlyExitException()

                        while stream_id.hasNext():
                            self._adaptor.addChild(root_1, stream_id.nextTree())


                        stream_id.reset()

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0


                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "ruleScopeSpec"

    class block_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.block_return, self).__init__()

            self.tree = None




    # $ANTLR start "block"
    # /home/res/ANTLRv3_py.g:188:1: block : lp= '(' ( (opts= optionsSpec )? ':' )? a1= alternative rewrite ( '|' a2= alternative rewrite )* rp= ')' -> ^( BLOCK[$lp,\"BLOCK\"] ( optionsSpec )? ( alternative ( rewrite )? )+ EOB[$rp,\"EOB\"] ) ;
    def block(self, ):

        retval = self.block_return()
        retval.start = self.input.LT(1)

        root_0 = None

        lp = None
        rp = None
        char_literal76 = None
        char_literal78 = None
        opts = None

        a1 = None

        a2 = None

        rewrite77 = None

        rewrite79 = None


        lp_tree = None
        rp_tree = None
        char_literal76_tree = None
        char_literal78_tree = None
        stream_79 = RewriteRuleTokenStream(self._adaptor, "token 79")
        stream_82 = RewriteRuleTokenStream(self._adaptor, "token 82")
        stream_83 = RewriteRuleTokenStream(self._adaptor, "token 83")
        stream_84 = RewriteRuleTokenStream(self._adaptor, "token 84")
        stream_rewrite = RewriteRuleSubtreeStream(self._adaptor, "rule rewrite")
        stream_alternative = RewriteRuleSubtreeStream(self._adaptor, "rule alternative")
        stream_optionsSpec = RewriteRuleSubtreeStream(self._adaptor, "rule optionsSpec")
        try:
            try:
                # /home/res/ANTLRv3_py.g:189:5: (lp= '(' ( (opts= optionsSpec )? ':' )? a1= alternative rewrite ( '|' a2= alternative rewrite )* rp= ')' -> ^( BLOCK[$lp,\"BLOCK\"] ( optionsSpec )? ( alternative ( rewrite )? )+ EOB[$rp,\"EOB\"] ) )
                # /home/res/ANTLRv3_py.g:189:9: lp= '(' ( (opts= optionsSpec )? ':' )? a1= alternative rewrite ( '|' a2= alternative rewrite )* rp= ')'
                pass 
                lp=self.match(self.input, 82, self.FOLLOW_82_in_block1184) 
                if self._state.backtracking == 0:
                    stream_82.add(lp)
                # /home/res/ANTLRv3_py.g:190:3: ( (opts= optionsSpec )? ':' )?
                alt31 = 2
                LA31_0 = self.input.LA(1)

                if (LA31_0 == OPTIONS or LA31_0 == 79) :
                    alt31 = 1
                if alt31 == 1:
                    # /home/res/ANTLRv3_py.g:190:5: (opts= optionsSpec )? ':'
                    pass 
                    # /home/res/ANTLRv3_py.g:190:5: (opts= optionsSpec )?
                    alt30 = 2
                    LA30_0 = self.input.LA(1)

                    if (LA30_0 == OPTIONS) :
                        alt30 = 1
                    if alt30 == 1:
                        # /home/res/ANTLRv3_py.g:190:6: opts= optionsSpec
                        pass 
                        self._state.following.append(self.FOLLOW_optionsSpec_in_block1193)
                        opts = self.optionsSpec()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_optionsSpec.add(opts.tree)



                    char_literal76=self.match(self.input, 79, self.FOLLOW_79_in_block1197) 
                    if self._state.backtracking == 0:
                        stream_79.add(char_literal76)



                self._state.following.append(self.FOLLOW_alternative_in_block1206)
                a1 = self.alternative()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_alternative.add(a1.tree)
                self._state.following.append(self.FOLLOW_rewrite_in_block1208)
                rewrite77 = self.rewrite()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_rewrite.add(rewrite77.tree)
                # /home/res/ANTLRv3_py.g:191:26: ( '|' a2= alternative rewrite )*
                while True: #loop32
                    alt32 = 2
                    LA32_0 = self.input.LA(1)

                    if (LA32_0 == 83) :
                        alt32 = 1


                    if alt32 == 1:
                        # /home/res/ANTLRv3_py.g:191:28: '|' a2= alternative rewrite
                        pass 
                        char_literal78=self.match(self.input, 83, self.FOLLOW_83_in_block1212) 
                        if self._state.backtracking == 0:
                            stream_83.add(char_literal78)
                        self._state.following.append(self.FOLLOW_alternative_in_block1216)
                        a2 = self.alternative()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_alternative.add(a2.tree)
                        self._state.following.append(self.FOLLOW_rewrite_in_block1218)
                        rewrite79 = self.rewrite()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_rewrite.add(rewrite79.tree)


                    else:
                        break #loop32
                rp=self.match(self.input, 84, self.FOLLOW_84_in_block1233) 
                if self._state.backtracking == 0:
                    stream_84.add(rp)

                # AST Rewrite
                # elements: optionsSpec, rewrite, alternative
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 193:9: -> ^( BLOCK[$lp,\"BLOCK\"] ( optionsSpec )? ( alternative ( rewrite )? )+ EOB[$rp,\"EOB\"] )
                    # /home/res/ANTLRv3_py.g:193:12: ^( BLOCK[$lp,\"BLOCK\"] ( optionsSpec )? ( alternative ( rewrite )? )+ EOB[$rp,\"EOB\"] )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(self._adaptor.create(BLOCK, lp, "BLOCK"), root_1)

                    # /home/res/ANTLRv3_py.g:193:34: ( optionsSpec )?
                    if stream_optionsSpec.hasNext():
                        self._adaptor.addChild(root_1, stream_optionsSpec.nextTree())


                    stream_optionsSpec.reset();
                    # /home/res/ANTLRv3_py.g:193:47: ( alternative ( rewrite )? )+
                    if not (stream_alternative.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_alternative.hasNext():
                        self._adaptor.addChild(root_1, stream_alternative.nextTree())
                        # /home/res/ANTLRv3_py.g:193:60: ( rewrite )?
                        if stream_rewrite.hasNext():
                            self._adaptor.addChild(root_1, stream_rewrite.nextTree())


                        stream_rewrite.reset();


                    stream_alternative.reset()
                    self._adaptor.addChild(root_1, self._adaptor.create(EOB, rp, "EOB"))

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "block"

    class altList_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.altList_return, self).__init__()

            self.tree = None




    # $ANTLR start "altList"
    # /home/res/ANTLRv3_py.g:196:1: altList : a1= alternative rewrite ( '|' a2= alternative rewrite )* -> ^( ( alternative ( rewrite )? )+ EOB[\"EOB\"] ) ;
    def altList(self, ):

        retval = self.altList_return()
        retval.start = self.input.LT(1)

        root_0 = None

        char_literal81 = None
        a1 = None

        a2 = None

        rewrite80 = None

        rewrite82 = None


        char_literal81_tree = None
        stream_83 = RewriteRuleTokenStream(self._adaptor, "token 83")
        stream_rewrite = RewriteRuleSubtreeStream(self._adaptor, "rule rewrite")
        stream_alternative = RewriteRuleSubtreeStream(self._adaptor, "rule alternative")
               
        #	// must create root manually as it's used by invoked rules in real antlr tool.
        #	// leave here to demonstrate use of {...} in rewrite rule
        #	// it's really BLOCK[firstToken,"BLOCK"]; set line/col to previous ( or : token.
        #    CommonTree blkRoot = (CommonTree)adaptor.create(BLOCK,input.LT(-1),"BLOCK");
        blkRoot = self.adaptor.create(BLOCK,self.input.LT(-1),"BLOCK");

        try:
            try:
                # /home/res/ANTLRv3_py.g:204:5: (a1= alternative rewrite ( '|' a2= alternative rewrite )* -> ^( ( alternative ( rewrite )? )+ EOB[\"EOB\"] ) )
                # /home/res/ANTLRv3_py.g:204:9: a1= alternative rewrite ( '|' a2= alternative rewrite )*
                pass 
                self._state.following.append(self.FOLLOW_alternative_in_altList1290)
                a1 = self.alternative()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_alternative.add(a1.tree)
                self._state.following.append(self.FOLLOW_rewrite_in_altList1292)
                rewrite80 = self.rewrite()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_rewrite.add(rewrite80.tree)
                # /home/res/ANTLRv3_py.g:204:32: ( '|' a2= alternative rewrite )*
                while True: #loop33
                    alt33 = 2
                    LA33_0 = self.input.LA(1)

                    if (LA33_0 == 83) :
                        alt33 = 1


                    if alt33 == 1:
                        # /home/res/ANTLRv3_py.g:204:34: '|' a2= alternative rewrite
                        pass 
                        char_literal81=self.match(self.input, 83, self.FOLLOW_83_in_altList1296) 
                        if self._state.backtracking == 0:
                            stream_83.add(char_literal81)
                        self._state.following.append(self.FOLLOW_alternative_in_altList1300)
                        a2 = self.alternative()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_alternative.add(a2.tree)
                        self._state.following.append(self.FOLLOW_rewrite_in_altList1302)
                        rewrite82 = self.rewrite()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_rewrite.add(rewrite82.tree)


                    else:
                        break #loop33

                # AST Rewrite
                # elements: alternative, rewrite
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 205:3: -> ^( ( alternative ( rewrite )? )+ EOB[\"EOB\"] )
                    # /home/res/ANTLRv3_py.g:205:6: ^( ( alternative ( rewrite )? )+ EOB[\"EOB\"] )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(blkRoot, root_1)

                    # /home/res/ANTLRv3_py.g:205:19: ( alternative ( rewrite )? )+
                    if not (stream_alternative.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_alternative.hasNext():
                        self._adaptor.addChild(root_1, stream_alternative.nextTree())
                        # /home/res/ANTLRv3_py.g:205:32: ( rewrite )?
                        if stream_rewrite.hasNext():
                            self._adaptor.addChild(root_1, stream_rewrite.nextTree())


                        stream_rewrite.reset();


                    stream_alternative.reset()
                    self._adaptor.addChild(root_1, self._adaptor.create(EOB, "EOB"))

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "altList"

    class alternative_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.alternative_return, self).__init__()

            self.tree = None




    # $ANTLR start "alternative"
    # /home/res/ANTLRv3_py.g:208:1: alternative : ( ( element )+ -> ^( ALT[firstToken,\"ALT\"] ( element )+ EOA[\"EOA\"] ) | -> ^( ALT[prevToken,\"ALT\"] EPSILON[prevToken,\"EPSILON\"] EOA[\"EOA\"] ) );
    def alternative(self, ):

        retval = self.alternative_return()
        retval.start = self.input.LT(1)

        root_0 = None

        element83 = None


        stream_element = RewriteRuleSubtreeStream(self._adaptor, "rule element")
               
        #	Token firstToken = input.LT(1);
        firstToken = self.input.LT(1)
        #	Token prevToken = input.LT(-1); // either : or | I think
        prevToken = self.input.LT(-1)

        try:
            try:
                # /home/res/ANTLRv3_py.g:215:5: ( ( element )+ -> ^( ALT[firstToken,\"ALT\"] ( element )+ EOA[\"EOA\"] ) | -> ^( ALT[prevToken,\"ALT\"] EPSILON[prevToken,\"EPSILON\"] EOA[\"EOA\"] ) )
                alt35 = 2
                LA35_0 = self.input.LA(1)

                if (LA35_0 == SEMPRED or LA35_0 == TREE_BEGIN or (TOKEN_REF <= LA35_0 <= ACTION) or LA35_0 == RULE_REF or LA35_0 == 82 or LA35_0 == 89 or LA35_0 == 92) :
                    alt35 = 1
                elif (LA35_0 == REWRITE or LA35_0 == 69 or (83 <= LA35_0 <= 84)) :
                    alt35 = 2
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed

                    nvae = NoViableAltException("", 35, 0, self.input)

                    raise nvae

                if alt35 == 1:
                    # /home/res/ANTLRv3_py.g:215:9: ( element )+
                    pass 
                    # /home/res/ANTLRv3_py.g:215:9: ( element )+
                    cnt34 = 0
                    while True: #loop34
                        alt34 = 2
                        LA34_0 = self.input.LA(1)

                        if (LA34_0 == SEMPRED or LA34_0 == TREE_BEGIN or (TOKEN_REF <= LA34_0 <= ACTION) or LA34_0 == RULE_REF or LA34_0 == 82 or LA34_0 == 89 or LA34_0 == 92) :
                            alt34 = 1


                        if alt34 == 1:
                            # /home/res/ANTLRv3_py.g:215:9: element
                            pass 
                            self._state.following.append(self.FOLLOW_element_in_alternative1350)
                            element83 = self.element()

                            self._state.following.pop()
                            if self._state.backtracking == 0:
                                stream_element.add(element83.tree)


                        else:
                            if cnt34 >= 1:
                                break #loop34

                            if self._state.backtracking > 0:
                                raise BacktrackingFailed

                            eee = EarlyExitException(34, self.input)
                            raise eee

                        cnt34 += 1

                    # AST Rewrite
                    # elements: element
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 215:18: -> ^( ALT[firstToken,\"ALT\"] ( element )+ EOA[\"EOA\"] )
                        # /home/res/ANTLRv3_py.g:215:21: ^( ALT[firstToken,\"ALT\"] ( element )+ EOA[\"EOA\"] )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(self._adaptor.create(ALT, firstToken, "ALT"), root_1)

                        # /home/res/ANTLRv3_py.g:215:45: ( element )+
                        if not (stream_element.hasNext()):
                            raise RewriteEarlyExitException()

                        while stream_element.hasNext():
                            self._adaptor.addChild(root_1, stream_element.nextTree())


                        stream_element.reset()
                        self._adaptor.addChild(root_1, self._adaptor.create(EOA, "EOA"))

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0


                elif alt35 == 2:
                    # /home/res/ANTLRv3_py.g:216:9: 
                    pass 
                    # AST Rewrite
                    # elements: 
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 216:9: -> ^( ALT[prevToken,\"ALT\"] EPSILON[prevToken,\"EPSILON\"] EOA[\"EOA\"] )
                        # /home/res/ANTLRv3_py.g:216:12: ^( ALT[prevToken,\"ALT\"] EPSILON[prevToken,\"EPSILON\"] EOA[\"EOA\"] )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(self._adaptor.create(ALT, prevToken, "ALT"), root_1)

                        self._adaptor.addChild(root_1, self._adaptor.create(EPSILON, prevToken, "EPSILON"))
                        self._adaptor.addChild(root_1, self._adaptor.create(EOA, "EOA"))

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0


                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "alternative"

    class exceptionGroup_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.exceptionGroup_return, self).__init__()

            self.tree = None




    # $ANTLR start "exceptionGroup"
    # /home/res/ANTLRv3_py.g:219:1: exceptionGroup : ( ( exceptionHandler )+ ( finallyClause )? | finallyClause );
    def exceptionGroup(self, ):

        retval = self.exceptionGroup_return()
        retval.start = self.input.LT(1)

        root_0 = None

        exceptionHandler84 = None

        finallyClause85 = None

        finallyClause86 = None



        try:
            try:
                # /home/res/ANTLRv3_py.g:220:2: ( ( exceptionHandler )+ ( finallyClause )? | finallyClause )
                alt38 = 2
                LA38_0 = self.input.LA(1)

                if (LA38_0 == 85) :
                    alt38 = 1
                elif (LA38_0 == 86) :
                    alt38 = 2
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed

                    nvae = NoViableAltException("", 38, 0, self.input)

                    raise nvae

                if alt38 == 1:
                    # /home/res/ANTLRv3_py.g:220:4: ( exceptionHandler )+ ( finallyClause )?
                    pass 
                    root_0 = self._adaptor.nil()

                    # /home/res/ANTLRv3_py.g:220:4: ( exceptionHandler )+
                    cnt36 = 0
                    while True: #loop36
                        alt36 = 2
                        LA36_0 = self.input.LA(1)

                        if (LA36_0 == 85) :
                            alt36 = 1


                        if alt36 == 1:
                            # /home/res/ANTLRv3_py.g:220:6: exceptionHandler
                            pass 
                            self._state.following.append(self.FOLLOW_exceptionHandler_in_exceptionGroup1401)
                            exceptionHandler84 = self.exceptionHandler()

                            self._state.following.pop()
                            if self._state.backtracking == 0:
                                self._adaptor.addChild(root_0, exceptionHandler84.tree)


                        else:
                            if cnt36 >= 1:
                                break #loop36

                            if self._state.backtracking > 0:
                                raise BacktrackingFailed

                            eee = EarlyExitException(36, self.input)
                            raise eee

                        cnt36 += 1
                    # /home/res/ANTLRv3_py.g:220:26: ( finallyClause )?
                    alt37 = 2
                    LA37_0 = self.input.LA(1)

                    if (LA37_0 == 86) :
                        alt37 = 1
                    if alt37 == 1:
                        # /home/res/ANTLRv3_py.g:220:28: finallyClause
                        pass 
                        self._state.following.append(self.FOLLOW_finallyClause_in_exceptionGroup1408)
                        finallyClause85 = self.finallyClause()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            self._adaptor.addChild(root_0, finallyClause85.tree)





                elif alt38 == 2:
                    # /home/res/ANTLRv3_py.g:221:4: finallyClause
                    pass 
                    root_0 = self._adaptor.nil()

                    self._state.following.append(self.FOLLOW_finallyClause_in_exceptionGroup1416)
                    finallyClause86 = self.finallyClause()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, finallyClause86.tree)


                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "exceptionGroup"

    class exceptionHandler_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.exceptionHandler_return, self).__init__()

            self.tree = None




    # $ANTLR start "exceptionHandler"
    # /home/res/ANTLRv3_py.g:224:1: exceptionHandler : 'catch' ARG_ACTION ACTION -> ^( 'catch' ARG_ACTION ACTION ) ;
    def exceptionHandler(self, ):

        retval = self.exceptionHandler_return()
        retval.start = self.input.LT(1)

        root_0 = None

        string_literal87 = None
        ARG_ACTION88 = None
        ACTION89 = None

        string_literal87_tree = None
        ARG_ACTION88_tree = None
        ACTION89_tree = None
        stream_ACTION = RewriteRuleTokenStream(self._adaptor, "token ACTION")
        stream_ARG_ACTION = RewriteRuleTokenStream(self._adaptor, "token ARG_ACTION")
        stream_85 = RewriteRuleTokenStream(self._adaptor, "token 85")

        try:
            try:
                # /home/res/ANTLRv3_py.g:225:5: ( 'catch' ARG_ACTION ACTION -> ^( 'catch' ARG_ACTION ACTION ) )
                # /home/res/ANTLRv3_py.g:225:10: 'catch' ARG_ACTION ACTION
                pass 
                string_literal87=self.match(self.input, 85, self.FOLLOW_85_in_exceptionHandler1436) 
                if self._state.backtracking == 0:
                    stream_85.add(string_literal87)
                ARG_ACTION88=self.match(self.input, ARG_ACTION, self.FOLLOW_ARG_ACTION_in_exceptionHandler1438) 
                if self._state.backtracking == 0:
                    stream_ARG_ACTION.add(ARG_ACTION88)
                ACTION89=self.match(self.input, ACTION, self.FOLLOW_ACTION_in_exceptionHandler1440) 
                if self._state.backtracking == 0:
                    stream_ACTION.add(ACTION89)

                # AST Rewrite
                # elements: ACTION, 85, ARG_ACTION
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 225:36: -> ^( 'catch' ARG_ACTION ACTION )
                    # /home/res/ANTLRv3_py.g:225:39: ^( 'catch' ARG_ACTION ACTION )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(stream_85.nextNode(), root_1)

                    self._adaptor.addChild(root_1, stream_ARG_ACTION.nextNode())
                    self._adaptor.addChild(root_1, stream_ACTION.nextNode())

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "exceptionHandler"

    class finallyClause_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.finallyClause_return, self).__init__()

            self.tree = None




    # $ANTLR start "finallyClause"
    # /home/res/ANTLRv3_py.g:228:1: finallyClause : 'finally' ACTION -> ^( 'finally' ACTION ) ;
    def finallyClause(self, ):

        retval = self.finallyClause_return()
        retval.start = self.input.LT(1)

        root_0 = None

        string_literal90 = None
        ACTION91 = None

        string_literal90_tree = None
        ACTION91_tree = None
        stream_ACTION = RewriteRuleTokenStream(self._adaptor, "token ACTION")
        stream_86 = RewriteRuleTokenStream(self._adaptor, "token 86")

        try:
            try:
                # /home/res/ANTLRv3_py.g:229:5: ( 'finally' ACTION -> ^( 'finally' ACTION ) )
                # /home/res/ANTLRv3_py.g:229:10: 'finally' ACTION
                pass 
                string_literal90=self.match(self.input, 86, self.FOLLOW_86_in_finallyClause1470) 
                if self._state.backtracking == 0:
                    stream_86.add(string_literal90)
                ACTION91=self.match(self.input, ACTION, self.FOLLOW_ACTION_in_finallyClause1472) 
                if self._state.backtracking == 0:
                    stream_ACTION.add(ACTION91)

                # AST Rewrite
                # elements: ACTION, 86
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 229:27: -> ^( 'finally' ACTION )
                    # /home/res/ANTLRv3_py.g:229:30: ^( 'finally' ACTION )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(stream_86.nextNode(), root_1)

                    self._adaptor.addChild(root_1, stream_ACTION.nextNode())

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "finallyClause"

    class element_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.element_return, self).__init__()

            self.tree = None




    # $ANTLR start "element"
    # /home/res/ANTLRv3_py.g:232:1: element : elementNoOptionSpec ;
    def element(self, ):

        retval = self.element_return()
        retval.start = self.input.LT(1)

        root_0 = None

        elementNoOptionSpec92 = None



        try:
            try:
                # /home/res/ANTLRv3_py.g:233:2: ( elementNoOptionSpec )
                # /home/res/ANTLRv3_py.g:233:4: elementNoOptionSpec
                pass 
                root_0 = self._adaptor.nil()

                self._state.following.append(self.FOLLOW_elementNoOptionSpec_in_element1494)
                elementNoOptionSpec92 = self.elementNoOptionSpec()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    self._adaptor.addChild(root_0, elementNoOptionSpec92.tree)



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "element"

    class elementNoOptionSpec_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.elementNoOptionSpec_return, self).__init__()

            self.tree = None




    # $ANTLR start "elementNoOptionSpec"
    # /home/res/ANTLRv3_py.g:236:1: elementNoOptionSpec : ( id (labelOp= '=' | labelOp= '+=' ) atom ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] ^( $labelOp id atom ) EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> ^( $labelOp id atom ) ) | id (labelOp= '=' | labelOp= '+=' ) block ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] ^( $labelOp id block ) EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> ^( $labelOp id block ) ) | atom ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] atom EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> atom ) | ebnf | ACTION | SEMPRED ( '=>' -> GATED_SEMPRED | -> SEMPRED ) | treeSpec ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] treeSpec EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> treeSpec ) );
    def elementNoOptionSpec(self, ):

        retval = self.elementNoOptionSpec_return()
        retval.start = self.input.LT(1)

        root_0 = None

        labelOp = None
        ACTION102 = None
        SEMPRED103 = None
        string_literal104 = None
        id93 = None

        atom94 = None

        ebnfSuffix95 = None

        id96 = None

        block97 = None

        ebnfSuffix98 = None

        atom99 = None

        ebnfSuffix100 = None

        ebnf101 = None

        treeSpec105 = None

        ebnfSuffix106 = None


        labelOp_tree = None
        ACTION102_tree = None
        SEMPRED103_tree = None
        string_literal104_tree = None
        stream_SEMPRED = RewriteRuleTokenStream(self._adaptor, "token SEMPRED")
        stream_71 = RewriteRuleTokenStream(self._adaptor, "token 71")
        stream_87 = RewriteRuleTokenStream(self._adaptor, "token 87")
        stream_88 = RewriteRuleTokenStream(self._adaptor, "token 88")
        stream_id = RewriteRuleSubtreeStream(self._adaptor, "rule id")
        stream_atom = RewriteRuleSubtreeStream(self._adaptor, "rule atom")
        stream_ebnfSuffix = RewriteRuleSubtreeStream(self._adaptor, "rule ebnfSuffix")
        stream_block = RewriteRuleSubtreeStream(self._adaptor, "rule block")
        stream_treeSpec = RewriteRuleSubtreeStream(self._adaptor, "rule treeSpec")
        try:
            try:
                # /home/res/ANTLRv3_py.g:237:2: ( id (labelOp= '=' | labelOp= '+=' ) atom ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] ^( $labelOp id atom ) EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> ^( $labelOp id atom ) ) | id (labelOp= '=' | labelOp= '+=' ) block ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] ^( $labelOp id block ) EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> ^( $labelOp id block ) ) | atom ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] atom EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> atom ) | ebnf | ACTION | SEMPRED ( '=>' -> GATED_SEMPRED | -> SEMPRED ) | treeSpec ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] treeSpec EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> treeSpec ) )
                alt46 = 7
                alt46 = self.dfa46.predict(self.input)
                if alt46 == 1:
                    # /home/res/ANTLRv3_py.g:237:4: id (labelOp= '=' | labelOp= '+=' ) atom ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] ^( $labelOp id atom ) EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> ^( $labelOp id atom ) )
                    pass 
                    self._state.following.append(self.FOLLOW_id_in_elementNoOptionSpec1505)
                    id93 = self.id()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_id.add(id93.tree)
                    # /home/res/ANTLRv3_py.g:237:7: (labelOp= '=' | labelOp= '+=' )
                    alt39 = 2
                    LA39_0 = self.input.LA(1)

                    if (LA39_0 == 71) :
                        alt39 = 1
                    elif (LA39_0 == 87) :
                        alt39 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        nvae = NoViableAltException("", 39, 0, self.input)

                        raise nvae

                    if alt39 == 1:
                        # /home/res/ANTLRv3_py.g:237:8: labelOp= '='
                        pass 
                        labelOp=self.match(self.input, 71, self.FOLLOW_71_in_elementNoOptionSpec1510) 
                        if self._state.backtracking == 0:
                            stream_71.add(labelOp)


                    elif alt39 == 2:
                        # /home/res/ANTLRv3_py.g:237:20: labelOp= '+='
                        pass 
                        labelOp=self.match(self.input, 87, self.FOLLOW_87_in_elementNoOptionSpec1514) 
                        if self._state.backtracking == 0:
                            stream_87.add(labelOp)



                    self._state.following.append(self.FOLLOW_atom_in_elementNoOptionSpec1517)
                    atom94 = self.atom()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_atom.add(atom94.tree)
                    # /home/res/ANTLRv3_py.g:238:3: ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] ^( $labelOp id atom ) EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> ^( $labelOp id atom ) )
                    alt40 = 2
                    LA40_0 = self.input.LA(1)

                    if (LA40_0 == 74 or (90 <= LA40_0 <= 91)) :
                        alt40 = 1
                    elif (LA40_0 == SEMPRED or LA40_0 == TREE_BEGIN or LA40_0 == REWRITE or (TOKEN_REF <= LA40_0 <= ACTION) or LA40_0 == RULE_REF or LA40_0 == 69 or (82 <= LA40_0 <= 84) or LA40_0 == 89 or LA40_0 == 92) :
                        alt40 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        nvae = NoViableAltException("", 40, 0, self.input)

                        raise nvae

                    if alt40 == 1:
                        # /home/res/ANTLRv3_py.g:238:5: ebnfSuffix
                        pass 
                        self._state.following.append(self.FOLLOW_ebnfSuffix_in_elementNoOptionSpec1523)
                        ebnfSuffix95 = self.ebnfSuffix()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_ebnfSuffix.add(ebnfSuffix95.tree)

                        # AST Rewrite
                        # elements: id, atom, labelOp, ebnfSuffix
                        # token labels: labelOp
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0
                            stream_labelOp = RewriteRuleTokenStream(self._adaptor, "token labelOp", labelOp)

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 238:16: -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] ^( $labelOp id atom ) EOA[\"EOA\"] ) EOB[\"EOB\"] ) )
                            # /home/res/ANTLRv3_py.g:238:19: ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] ^( $labelOp id atom ) EOA[\"EOA\"] ) EOB[\"EOB\"] ) )
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(stream_ebnfSuffix.nextNode(), root_1)

                            # /home/res/ANTLRv3_py.g:238:33: ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] ^( $labelOp id atom ) EOA[\"EOA\"] ) EOB[\"EOB\"] )
                            root_2 = self._adaptor.nil()
                            root_2 = self._adaptor.becomeRoot(self._adaptor.create(BLOCK, "BLOCK"), root_2)

                            # /home/res/ANTLRv3_py.g:238:50: ^( ALT[\"ALT\"] ^( $labelOp id atom ) EOA[\"EOA\"] )
                            root_3 = self._adaptor.nil()
                            root_3 = self._adaptor.becomeRoot(self._adaptor.create(ALT, "ALT"), root_3)

                            # /home/res/ANTLRv3_py.g:238:63: ^( $labelOp id atom )
                            root_4 = self._adaptor.nil()
                            root_4 = self._adaptor.becomeRoot(stream_labelOp.nextNode(), root_4)

                            self._adaptor.addChild(root_4, stream_id.nextTree())
                            self._adaptor.addChild(root_4, stream_atom.nextTree())

                            self._adaptor.addChild(root_3, root_4)
                            self._adaptor.addChild(root_3, self._adaptor.create(EOA, "EOA"))

                            self._adaptor.addChild(root_2, root_3)
                            self._adaptor.addChild(root_2, self._adaptor.create(EOB, "EOB"))

                            self._adaptor.addChild(root_1, root_2)

                            self._adaptor.addChild(root_0, root_1)



                            retval.tree = root_0


                    elif alt40 == 2:
                        # /home/res/ANTLRv3_py.g:239:8: 
                        pass 
                        # AST Rewrite
                        # elements: id, atom, labelOp
                        # token labels: labelOp
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0
                            stream_labelOp = RewriteRuleTokenStream(self._adaptor, "token labelOp", labelOp)

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 239:8: -> ^( $labelOp id atom )
                            # /home/res/ANTLRv3_py.g:239:11: ^( $labelOp id atom )
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(stream_labelOp.nextNode(), root_1)

                            self._adaptor.addChild(root_1, stream_id.nextTree())
                            self._adaptor.addChild(root_1, stream_atom.nextTree())

                            self._adaptor.addChild(root_0, root_1)



                            retval.tree = root_0





                elif alt46 == 2:
                    # /home/res/ANTLRv3_py.g:241:4: id (labelOp= '=' | labelOp= '+=' ) block ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] ^( $labelOp id block ) EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> ^( $labelOp id block ) )
                    pass 
                    self._state.following.append(self.FOLLOW_id_in_elementNoOptionSpec1582)
                    id96 = self.id()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_id.add(id96.tree)
                    # /home/res/ANTLRv3_py.g:241:7: (labelOp= '=' | labelOp= '+=' )
                    alt41 = 2
                    LA41_0 = self.input.LA(1)

                    if (LA41_0 == 71) :
                        alt41 = 1
                    elif (LA41_0 == 87) :
                        alt41 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        nvae = NoViableAltException("", 41, 0, self.input)

                        raise nvae

                    if alt41 == 1:
                        # /home/res/ANTLRv3_py.g:241:8: labelOp= '='
                        pass 
                        labelOp=self.match(self.input, 71, self.FOLLOW_71_in_elementNoOptionSpec1587) 
                        if self._state.backtracking == 0:
                            stream_71.add(labelOp)


                    elif alt41 == 2:
                        # /home/res/ANTLRv3_py.g:241:20: labelOp= '+='
                        pass 
                        labelOp=self.match(self.input, 87, self.FOLLOW_87_in_elementNoOptionSpec1591) 
                        if self._state.backtracking == 0:
                            stream_87.add(labelOp)



                    self._state.following.append(self.FOLLOW_block_in_elementNoOptionSpec1594)
                    block97 = self.block()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_block.add(block97.tree)
                    # /home/res/ANTLRv3_py.g:242:3: ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] ^( $labelOp id block ) EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> ^( $labelOp id block ) )
                    alt42 = 2
                    LA42_0 = self.input.LA(1)

                    if (LA42_0 == 74 or (90 <= LA42_0 <= 91)) :
                        alt42 = 1
                    elif (LA42_0 == SEMPRED or LA42_0 == TREE_BEGIN or LA42_0 == REWRITE or (TOKEN_REF <= LA42_0 <= ACTION) or LA42_0 == RULE_REF or LA42_0 == 69 or (82 <= LA42_0 <= 84) or LA42_0 == 89 or LA42_0 == 92) :
                        alt42 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        nvae = NoViableAltException("", 42, 0, self.input)

                        raise nvae

                    if alt42 == 1:
                        # /home/res/ANTLRv3_py.g:242:5: ebnfSuffix
                        pass 
                        self._state.following.append(self.FOLLOW_ebnfSuffix_in_elementNoOptionSpec1600)
                        ebnfSuffix98 = self.ebnfSuffix()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_ebnfSuffix.add(ebnfSuffix98.tree)

                        # AST Rewrite
                        # elements: block, ebnfSuffix, labelOp, id
                        # token labels: labelOp
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0
                            stream_labelOp = RewriteRuleTokenStream(self._adaptor, "token labelOp", labelOp)

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 242:16: -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] ^( $labelOp id block ) EOA[\"EOA\"] ) EOB[\"EOB\"] ) )
                            # /home/res/ANTLRv3_py.g:242:19: ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] ^( $labelOp id block ) EOA[\"EOA\"] ) EOB[\"EOB\"] ) )
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(stream_ebnfSuffix.nextNode(), root_1)

                            # /home/res/ANTLRv3_py.g:242:33: ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] ^( $labelOp id block ) EOA[\"EOA\"] ) EOB[\"EOB\"] )
                            root_2 = self._adaptor.nil()
                            root_2 = self._adaptor.becomeRoot(self._adaptor.create(BLOCK, "BLOCK"), root_2)

                            # /home/res/ANTLRv3_py.g:242:50: ^( ALT[\"ALT\"] ^( $labelOp id block ) EOA[\"EOA\"] )
                            root_3 = self._adaptor.nil()
                            root_3 = self._adaptor.becomeRoot(self._adaptor.create(ALT, "ALT"), root_3)

                            # /home/res/ANTLRv3_py.g:242:63: ^( $labelOp id block )
                            root_4 = self._adaptor.nil()
                            root_4 = self._adaptor.becomeRoot(stream_labelOp.nextNode(), root_4)

                            self._adaptor.addChild(root_4, stream_id.nextTree())
                            self._adaptor.addChild(root_4, stream_block.nextTree())

                            self._adaptor.addChild(root_3, root_4)
                            self._adaptor.addChild(root_3, self._adaptor.create(EOA, "EOA"))

                            self._adaptor.addChild(root_2, root_3)
                            self._adaptor.addChild(root_2, self._adaptor.create(EOB, "EOB"))

                            self._adaptor.addChild(root_1, root_2)

                            self._adaptor.addChild(root_0, root_1)



                            retval.tree = root_0


                    elif alt42 == 2:
                        # /home/res/ANTLRv3_py.g:243:8: 
                        pass 
                        # AST Rewrite
                        # elements: id, block, labelOp
                        # token labels: labelOp
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0
                            stream_labelOp = RewriteRuleTokenStream(self._adaptor, "token labelOp", labelOp)

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 243:8: -> ^( $labelOp id block )
                            # /home/res/ANTLRv3_py.g:243:11: ^( $labelOp id block )
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(stream_labelOp.nextNode(), root_1)

                            self._adaptor.addChild(root_1, stream_id.nextTree())
                            self._adaptor.addChild(root_1, stream_block.nextTree())

                            self._adaptor.addChild(root_0, root_1)



                            retval.tree = root_0





                elif alt46 == 3:
                    # /home/res/ANTLRv3_py.g:245:4: atom ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] atom EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> atom )
                    pass 
                    self._state.following.append(self.FOLLOW_atom_in_elementNoOptionSpec1659)
                    atom99 = self.atom()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_atom.add(atom99.tree)
                    # /home/res/ANTLRv3_py.g:246:3: ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] atom EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> atom )
                    alt43 = 2
                    LA43_0 = self.input.LA(1)

                    if (LA43_0 == 74 or (90 <= LA43_0 <= 91)) :
                        alt43 = 1
                    elif (LA43_0 == SEMPRED or LA43_0 == TREE_BEGIN or LA43_0 == REWRITE or (TOKEN_REF <= LA43_0 <= ACTION) or LA43_0 == RULE_REF or LA43_0 == 69 or (82 <= LA43_0 <= 84) or LA43_0 == 89 or LA43_0 == 92) :
                        alt43 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        nvae = NoViableAltException("", 43, 0, self.input)

                        raise nvae

                    if alt43 == 1:
                        # /home/res/ANTLRv3_py.g:246:5: ebnfSuffix
                        pass 
                        self._state.following.append(self.FOLLOW_ebnfSuffix_in_elementNoOptionSpec1665)
                        ebnfSuffix100 = self.ebnfSuffix()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_ebnfSuffix.add(ebnfSuffix100.tree)

                        # AST Rewrite
                        # elements: ebnfSuffix, atom
                        # token labels: 
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 246:16: -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] atom EOA[\"EOA\"] ) EOB[\"EOB\"] ) )
                            # /home/res/ANTLRv3_py.g:246:19: ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] atom EOA[\"EOA\"] ) EOB[\"EOB\"] ) )
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(stream_ebnfSuffix.nextNode(), root_1)

                            # /home/res/ANTLRv3_py.g:246:33: ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] atom EOA[\"EOA\"] ) EOB[\"EOB\"] )
                            root_2 = self._adaptor.nil()
                            root_2 = self._adaptor.becomeRoot(self._adaptor.create(BLOCK, "BLOCK"), root_2)

                            # /home/res/ANTLRv3_py.g:246:50: ^( ALT[\"ALT\"] atom EOA[\"EOA\"] )
                            root_3 = self._adaptor.nil()
                            root_3 = self._adaptor.becomeRoot(self._adaptor.create(ALT, "ALT"), root_3)

                            self._adaptor.addChild(root_3, stream_atom.nextTree())
                            self._adaptor.addChild(root_3, self._adaptor.create(EOA, "EOA"))

                            self._adaptor.addChild(root_2, root_3)
                            self._adaptor.addChild(root_2, self._adaptor.create(EOB, "EOB"))

                            self._adaptor.addChild(root_1, root_2)

                            self._adaptor.addChild(root_0, root_1)



                            retval.tree = root_0


                    elif alt43 == 2:
                        # /home/res/ANTLRv3_py.g:247:8: 
                        pass 
                        # AST Rewrite
                        # elements: atom
                        # token labels: 
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 247:8: -> atom
                            self._adaptor.addChild(root_0, stream_atom.nextTree())



                            retval.tree = root_0





                elif alt46 == 4:
                    # /home/res/ANTLRv3_py.g:249:4: ebnf
                    pass 
                    root_0 = self._adaptor.nil()

                    self._state.following.append(self.FOLLOW_ebnf_in_elementNoOptionSpec1711)
                    ebnf101 = self.ebnf()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, ebnf101.tree)


                elif alt46 == 5:
                    # /home/res/ANTLRv3_py.g:250:6: ACTION
                    pass 
                    root_0 = self._adaptor.nil()

                    ACTION102=self.match(self.input, ACTION, self.FOLLOW_ACTION_in_elementNoOptionSpec1718)
                    if self._state.backtracking == 0:

                        ACTION102_tree = self._adaptor.createWithPayload(ACTION102)
                        self._adaptor.addChild(root_0, ACTION102_tree)



                elif alt46 == 6:
                    # /home/res/ANTLRv3_py.g:251:6: SEMPRED ( '=>' -> GATED_SEMPRED | -> SEMPRED )
                    pass 
                    SEMPRED103=self.match(self.input, SEMPRED, self.FOLLOW_SEMPRED_in_elementNoOptionSpec1725) 
                    if self._state.backtracking == 0:
                        stream_SEMPRED.add(SEMPRED103)
                    # /home/res/ANTLRv3_py.g:251:14: ( '=>' -> GATED_SEMPRED | -> SEMPRED )
                    alt44 = 2
                    LA44_0 = self.input.LA(1)

                    if (LA44_0 == 88) :
                        alt44 = 1
                    elif (LA44_0 == SEMPRED or LA44_0 == TREE_BEGIN or LA44_0 == REWRITE or (TOKEN_REF <= LA44_0 <= ACTION) or LA44_0 == RULE_REF or LA44_0 == 69 or (82 <= LA44_0 <= 84) or LA44_0 == 89 or LA44_0 == 92) :
                        alt44 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        nvae = NoViableAltException("", 44, 0, self.input)

                        raise nvae

                    if alt44 == 1:
                        # /home/res/ANTLRv3_py.g:251:16: '=>'
                        pass 
                        string_literal104=self.match(self.input, 88, self.FOLLOW_88_in_elementNoOptionSpec1729) 
                        if self._state.backtracking == 0:
                            stream_88.add(string_literal104)

                        # AST Rewrite
                        # elements: 
                        # token labels: 
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 251:21: -> GATED_SEMPRED
                            self._adaptor.addChild(root_0, self._adaptor.createFromType(GATED_SEMPRED, "GATED_SEMPRED"))



                            retval.tree = root_0


                    elif alt44 == 2:
                        # /home/res/ANTLRv3_py.g:251:40: 
                        pass 
                        # AST Rewrite
                        # elements: SEMPRED
                        # token labels: 
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 251:40: -> SEMPRED
                            self._adaptor.addChild(root_0, stream_SEMPRED.nextNode())



                            retval.tree = root_0





                elif alt46 == 7:
                    # /home/res/ANTLRv3_py.g:252:6: treeSpec ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] treeSpec EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> treeSpec )
                    pass 
                    self._state.following.append(self.FOLLOW_treeSpec_in_elementNoOptionSpec1748)
                    treeSpec105 = self.treeSpec()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_treeSpec.add(treeSpec105.tree)
                    # /home/res/ANTLRv3_py.g:253:3: ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] treeSpec EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> treeSpec )
                    alt45 = 2
                    LA45_0 = self.input.LA(1)

                    if (LA45_0 == 74 or (90 <= LA45_0 <= 91)) :
                        alt45 = 1
                    elif (LA45_0 == SEMPRED or LA45_0 == TREE_BEGIN or LA45_0 == REWRITE or (TOKEN_REF <= LA45_0 <= ACTION) or LA45_0 == RULE_REF or LA45_0 == 69 or (82 <= LA45_0 <= 84) or LA45_0 == 89 or LA45_0 == 92) :
                        alt45 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        nvae = NoViableAltException("", 45, 0, self.input)

                        raise nvae

                    if alt45 == 1:
                        # /home/res/ANTLRv3_py.g:253:5: ebnfSuffix
                        pass 
                        self._state.following.append(self.FOLLOW_ebnfSuffix_in_elementNoOptionSpec1754)
                        ebnfSuffix106 = self.ebnfSuffix()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_ebnfSuffix.add(ebnfSuffix106.tree)

                        # AST Rewrite
                        # elements: treeSpec, ebnfSuffix
                        # token labels: 
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 253:16: -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] treeSpec EOA[\"EOA\"] ) EOB[\"EOB\"] ) )
                            # /home/res/ANTLRv3_py.g:253:19: ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] treeSpec EOA[\"EOA\"] ) EOB[\"EOB\"] ) )
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(stream_ebnfSuffix.nextNode(), root_1)

                            # /home/res/ANTLRv3_py.g:253:33: ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] treeSpec EOA[\"EOA\"] ) EOB[\"EOB\"] )
                            root_2 = self._adaptor.nil()
                            root_2 = self._adaptor.becomeRoot(self._adaptor.create(BLOCK, "BLOCK"), root_2)

                            # /home/res/ANTLRv3_py.g:253:50: ^( ALT[\"ALT\"] treeSpec EOA[\"EOA\"] )
                            root_3 = self._adaptor.nil()
                            root_3 = self._adaptor.becomeRoot(self._adaptor.create(ALT, "ALT"), root_3)

                            self._adaptor.addChild(root_3, stream_treeSpec.nextTree())
                            self._adaptor.addChild(root_3, self._adaptor.create(EOA, "EOA"))

                            self._adaptor.addChild(root_2, root_3)
                            self._adaptor.addChild(root_2, self._adaptor.create(EOB, "EOB"))

                            self._adaptor.addChild(root_1, root_2)

                            self._adaptor.addChild(root_0, root_1)



                            retval.tree = root_0


                    elif alt45 == 2:
                        # /home/res/ANTLRv3_py.g:254:8: 
                        pass 
                        # AST Rewrite
                        # elements: treeSpec
                        # token labels: 
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 254:8: -> treeSpec
                            self._adaptor.addChild(root_0, stream_treeSpec.nextTree())



                            retval.tree = root_0





                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "elementNoOptionSpec"

    class atom_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.atom_return, self).__init__()

            self.tree = None




    # $ANTLR start "atom"
    # /home/res/ANTLRv3_py.g:258:1: atom : ( range ( (op= '^' | op= '!' ) -> ^( $op range ) | -> range ) | terminal | notSet ( (op= '^' | op= '!' ) -> ^( $op notSet ) | -> notSet ) | RULE_REF (arg= ARG_ACTION )? ( (op= '^' | op= '!' ) )? -> {$arg!=None and op!=None}? ^( $op RULE_REF $arg) -> {$arg!=None}? ^( RULE_REF $arg) -> {$op!=None}? ^( $op RULE_REF ) -> RULE_REF );
    def atom(self, ):

        retval = self.atom_return()
        retval.start = self.input.LT(1)

        root_0 = None

        op = None
        arg = None
        RULE_REF110 = None
        range107 = None

        terminal108 = None

        notSet109 = None


        op_tree = None
        arg_tree = None
        RULE_REF110_tree = None
        stream_BANG = RewriteRuleTokenStream(self._adaptor, "token BANG")
        stream_ROOT = RewriteRuleTokenStream(self._adaptor, "token ROOT")
        stream_RULE_REF = RewriteRuleTokenStream(self._adaptor, "token RULE_REF")
        stream_ARG_ACTION = RewriteRuleTokenStream(self._adaptor, "token ARG_ACTION")
        stream_range = RewriteRuleSubtreeStream(self._adaptor, "rule range")
        stream_notSet = RewriteRuleSubtreeStream(self._adaptor, "rule notSet")
        try:
            try:
                # /home/res/ANTLRv3_py.g:258:5: ( range ( (op= '^' | op= '!' ) -> ^( $op range ) | -> range ) | terminal | notSet ( (op= '^' | op= '!' ) -> ^( $op notSet ) | -> notSet ) | RULE_REF (arg= ARG_ACTION )? ( (op= '^' | op= '!' ) )? -> {$arg!=None and op!=None}? ^( $op RULE_REF $arg) -> {$arg!=None}? ^( RULE_REF $arg) -> {$op!=None}? ^( $op RULE_REF ) -> RULE_REF )
                alt54 = 4
                LA54 = self.input.LA(1)
                if LA54 == CHAR_LITERAL:
                    LA54_1 = self.input.LA(2)

                    if (LA54_1 == RANGE) :
                        alt54 = 1
                    elif (LA54_1 == SEMPRED or (TREE_BEGIN <= LA54_1 <= REWRITE) or (TOKEN_REF <= LA54_1 <= ACTION) or LA54_1 == RULE_REF or LA54_1 == 69 or LA54_1 == 74 or (82 <= LA54_1 <= 84) or (89 <= LA54_1 <= 92)) :
                        alt54 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        nvae = NoViableAltException("", 54, 1, self.input)

                        raise nvae

                elif LA54 == TOKEN_REF or LA54 == STRING_LITERAL or LA54 == 92:
                    alt54 = 2
                elif LA54 == 89:
                    alt54 = 3
                elif LA54 == RULE_REF:
                    alt54 = 4
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed

                    nvae = NoViableAltException("", 54, 0, self.input)

                    raise nvae

                if alt54 == 1:
                    # /home/res/ANTLRv3_py.g:258:9: range ( (op= '^' | op= '!' ) -> ^( $op range ) | -> range )
                    pass 
                    self._state.following.append(self.FOLLOW_range_in_atom1806)
                    range107 = self.range()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_range.add(range107.tree)
                    # /home/res/ANTLRv3_py.g:258:15: ( (op= '^' | op= '!' ) -> ^( $op range ) | -> range )
                    alt48 = 2
                    LA48_0 = self.input.LA(1)

                    if ((ROOT <= LA48_0 <= BANG)) :
                        alt48 = 1
                    elif (LA48_0 == SEMPRED or LA48_0 == TREE_BEGIN or LA48_0 == REWRITE or (TOKEN_REF <= LA48_0 <= ACTION) or LA48_0 == RULE_REF or LA48_0 == 69 or LA48_0 == 74 or (82 <= LA48_0 <= 84) or (89 <= LA48_0 <= 92)) :
                        alt48 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        nvae = NoViableAltException("", 48, 0, self.input)

                        raise nvae

                    if alt48 == 1:
                        # /home/res/ANTLRv3_py.g:258:17: (op= '^' | op= '!' )
                        pass 
                        # /home/res/ANTLRv3_py.g:258:17: (op= '^' | op= '!' )
                        alt47 = 2
                        LA47_0 = self.input.LA(1)

                        if (LA47_0 == ROOT) :
                            alt47 = 1
                        elif (LA47_0 == BANG) :
                            alt47 = 2
                        else:
                            if self._state.backtracking > 0:
                                raise BacktrackingFailed

                            nvae = NoViableAltException("", 47, 0, self.input)

                            raise nvae

                        if alt47 == 1:
                            # /home/res/ANTLRv3_py.g:258:18: op= '^'
                            pass 
                            op=self.match(self.input, ROOT, self.FOLLOW_ROOT_in_atom1813) 
                            if self._state.backtracking == 0:
                                stream_ROOT.add(op)


                        elif alt47 == 2:
                            # /home/res/ANTLRv3_py.g:258:25: op= '!'
                            pass 
                            op=self.match(self.input, BANG, self.FOLLOW_BANG_in_atom1817) 
                            if self._state.backtracking == 0:
                                stream_BANG.add(op)




                        # AST Rewrite
                        # elements: op, range
                        # token labels: op
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0
                            stream_op = RewriteRuleTokenStream(self._adaptor, "token op", op)

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 258:33: -> ^( $op range )
                            # /home/res/ANTLRv3_py.g:258:36: ^( $op range )
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(stream_op.nextNode(), root_1)

                            self._adaptor.addChild(root_1, stream_range.nextTree())

                            self._adaptor.addChild(root_0, root_1)



                            retval.tree = root_0


                    elif alt48 == 2:
                        # /home/res/ANTLRv3_py.g:258:51: 
                        pass 
                        # AST Rewrite
                        # elements: range
                        # token labels: 
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 258:51: -> range
                            self._adaptor.addChild(root_0, stream_range.nextTree())



                            retval.tree = root_0





                elif alt54 == 2:
                    # /home/res/ANTLRv3_py.g:259:9: terminal
                    pass 
                    root_0 = self._adaptor.nil()

                    self._state.following.append(self.FOLLOW_terminal_in_atom1845)
                    terminal108 = self.terminal()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, terminal108.tree)


                elif alt54 == 3:
                    # /home/res/ANTLRv3_py.g:260:7: notSet ( (op= '^' | op= '!' ) -> ^( $op notSet ) | -> notSet )
                    pass 
                    self._state.following.append(self.FOLLOW_notSet_in_atom1853)
                    notSet109 = self.notSet()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_notSet.add(notSet109.tree)
                    # /home/res/ANTLRv3_py.g:260:14: ( (op= '^' | op= '!' ) -> ^( $op notSet ) | -> notSet )
                    alt50 = 2
                    LA50_0 = self.input.LA(1)

                    if ((ROOT <= LA50_0 <= BANG)) :
                        alt50 = 1
                    elif (LA50_0 == SEMPRED or LA50_0 == TREE_BEGIN or LA50_0 == REWRITE or (TOKEN_REF <= LA50_0 <= ACTION) or LA50_0 == RULE_REF or LA50_0 == 69 or LA50_0 == 74 or (82 <= LA50_0 <= 84) or (89 <= LA50_0 <= 92)) :
                        alt50 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        nvae = NoViableAltException("", 50, 0, self.input)

                        raise nvae

                    if alt50 == 1:
                        # /home/res/ANTLRv3_py.g:260:16: (op= '^' | op= '!' )
                        pass 
                        # /home/res/ANTLRv3_py.g:260:16: (op= '^' | op= '!' )
                        alt49 = 2
                        LA49_0 = self.input.LA(1)

                        if (LA49_0 == ROOT) :
                            alt49 = 1
                        elif (LA49_0 == BANG) :
                            alt49 = 2
                        else:
                            if self._state.backtracking > 0:
                                raise BacktrackingFailed

                            nvae = NoViableAltException("", 49, 0, self.input)

                            raise nvae

                        if alt49 == 1:
                            # /home/res/ANTLRv3_py.g:260:17: op= '^'
                            pass 
                            op=self.match(self.input, ROOT, self.FOLLOW_ROOT_in_atom1860) 
                            if self._state.backtracking == 0:
                                stream_ROOT.add(op)


                        elif alt49 == 2:
                            # /home/res/ANTLRv3_py.g:260:24: op= '!'
                            pass 
                            op=self.match(self.input, BANG, self.FOLLOW_BANG_in_atom1864) 
                            if self._state.backtracking == 0:
                                stream_BANG.add(op)




                        # AST Rewrite
                        # elements: notSet, op
                        # token labels: op
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0
                            stream_op = RewriteRuleTokenStream(self._adaptor, "token op", op)

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 260:32: -> ^( $op notSet )
                            # /home/res/ANTLRv3_py.g:260:35: ^( $op notSet )
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(stream_op.nextNode(), root_1)

                            self._adaptor.addChild(root_1, stream_notSet.nextTree())

                            self._adaptor.addChild(root_0, root_1)



                            retval.tree = root_0


                    elif alt50 == 2:
                        # /home/res/ANTLRv3_py.g:260:51: 
                        pass 
                        # AST Rewrite
                        # elements: notSet
                        # token labels: 
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 260:51: -> notSet
                            self._adaptor.addChild(root_0, stream_notSet.nextTree())



                            retval.tree = root_0





                elif alt54 == 4:
                    # /home/res/ANTLRv3_py.g:261:9: RULE_REF (arg= ARG_ACTION )? ( (op= '^' | op= '!' ) )?
                    pass 
                    RULE_REF110=self.match(self.input, RULE_REF, self.FOLLOW_RULE_REF_in_atom1892) 
                    if self._state.backtracking == 0:
                        stream_RULE_REF.add(RULE_REF110)
                    # /home/res/ANTLRv3_py.g:261:18: (arg= ARG_ACTION )?
                    alt51 = 2
                    LA51_0 = self.input.LA(1)

                    if (LA51_0 == ARG_ACTION) :
                        alt51 = 1
                    if alt51 == 1:
                        # /home/res/ANTLRv3_py.g:261:20: arg= ARG_ACTION
                        pass 
                        arg=self.match(self.input, ARG_ACTION, self.FOLLOW_ARG_ACTION_in_atom1898) 
                        if self._state.backtracking == 0:
                            stream_ARG_ACTION.add(arg)



                    # /home/res/ANTLRv3_py.g:261:38: ( (op= '^' | op= '!' ) )?
                    alt53 = 2
                    LA53_0 = self.input.LA(1)

                    if ((ROOT <= LA53_0 <= BANG)) :
                        alt53 = 1
                    if alt53 == 1:
                        # /home/res/ANTLRv3_py.g:261:40: (op= '^' | op= '!' )
                        pass 
                        # /home/res/ANTLRv3_py.g:261:40: (op= '^' | op= '!' )
                        alt52 = 2
                        LA52_0 = self.input.LA(1)

                        if (LA52_0 == ROOT) :
                            alt52 = 1
                        elif (LA52_0 == BANG) :
                            alt52 = 2
                        else:
                            if self._state.backtracking > 0:
                                raise BacktrackingFailed

                            nvae = NoViableAltException("", 52, 0, self.input)

                            raise nvae

                        if alt52 == 1:
                            # /home/res/ANTLRv3_py.g:261:41: op= '^'
                            pass 
                            op=self.match(self.input, ROOT, self.FOLLOW_ROOT_in_atom1908) 
                            if self._state.backtracking == 0:
                                stream_ROOT.add(op)


                        elif alt52 == 2:
                            # /home/res/ANTLRv3_py.g:261:48: op= '!'
                            pass 
                            op=self.match(self.input, BANG, self.FOLLOW_BANG_in_atom1912) 
                            if self._state.backtracking == 0:
                                stream_BANG.add(op)







                    # AST Rewrite
                    # elements: RULE_REF, arg, RULE_REF, arg, RULE_REF, RULE_REF, op, op
                    # token labels: arg, op
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0
                        stream_arg = RewriteRuleTokenStream(self._adaptor, "token arg", arg)
                        stream_op = RewriteRuleTokenStream(self._adaptor, "token op", op)

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        if arg!=None and op!=None:
                            # 262:6: -> {$arg!=None and op!=None}? ^( $op RULE_REF $arg)
                            # /home/res/ANTLRv3_py.g:262:36: ^( $op RULE_REF $arg)
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(stream_op.nextNode(), root_1)

                            self._adaptor.addChild(root_1, stream_RULE_REF.nextNode())
                            self._adaptor.addChild(root_1, stream_arg.nextNode())

                            self._adaptor.addChild(root_0, root_1)


                        elif arg!=None:
                            # 263:6: -> {$arg!=None}? ^( RULE_REF $arg)
                            # /home/res/ANTLRv3_py.g:263:25: ^( RULE_REF $arg)
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(stream_RULE_REF.nextNode(), root_1)

                            self._adaptor.addChild(root_1, stream_arg.nextNode())

                            self._adaptor.addChild(root_0, root_1)


                        elif op!=None:
                            # 264:6: -> {$op!=None}? ^( $op RULE_REF )
                            # /home/res/ANTLRv3_py.g:264:25: ^( $op RULE_REF )
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(stream_op.nextNode(), root_1)

                            self._adaptor.addChild(root_1, stream_RULE_REF.nextNode())

                            self._adaptor.addChild(root_0, root_1)


                        else: 
                            # 265:6: -> RULE_REF
                            self._adaptor.addChild(root_0, stream_RULE_REF.nextNode())


                        retval.tree = root_0


                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "atom"

    class notSet_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.notSet_return, self).__init__()

            self.tree = None




    # $ANTLR start "notSet"
    # /home/res/ANTLRv3_py.g:268:1: notSet : '~' ( notTerminal -> ^( '~' notTerminal ) | block -> ^( '~' block ) ) ;
    def notSet(self, ):

        retval = self.notSet_return()
        retval.start = self.input.LT(1)

        root_0 = None

        char_literal111 = None
        notTerminal112 = None

        block113 = None


        char_literal111_tree = None
        stream_89 = RewriteRuleTokenStream(self._adaptor, "token 89")
        stream_notTerminal = RewriteRuleSubtreeStream(self._adaptor, "rule notTerminal")
        stream_block = RewriteRuleSubtreeStream(self._adaptor, "rule block")
        try:
            try:
                # /home/res/ANTLRv3_py.g:269:2: ( '~' ( notTerminal -> ^( '~' notTerminal ) | block -> ^( '~' block ) ) )
                # /home/res/ANTLRv3_py.g:269:4: '~' ( notTerminal -> ^( '~' notTerminal ) | block -> ^( '~' block ) )
                pass 
                char_literal111=self.match(self.input, 89, self.FOLLOW_89_in_notSet1995) 
                if self._state.backtracking == 0:
                    stream_89.add(char_literal111)
                # /home/res/ANTLRv3_py.g:270:3: ( notTerminal -> ^( '~' notTerminal ) | block -> ^( '~' block ) )
                alt55 = 2
                LA55_0 = self.input.LA(1)

                if ((TOKEN_REF <= LA55_0 <= CHAR_LITERAL)) :
                    alt55 = 1
                elif (LA55_0 == 82) :
                    alt55 = 2
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed

                    nvae = NoViableAltException("", 55, 0, self.input)

                    raise nvae

                if alt55 == 1:
                    # /home/res/ANTLRv3_py.g:270:5: notTerminal
                    pass 
                    self._state.following.append(self.FOLLOW_notTerminal_in_notSet2001)
                    notTerminal112 = self.notTerminal()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_notTerminal.add(notTerminal112.tree)

                    # AST Rewrite
                    # elements: notTerminal, 89
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 270:17: -> ^( '~' notTerminal )
                        # /home/res/ANTLRv3_py.g:270:20: ^( '~' notTerminal )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(stream_89.nextNode(), root_1)

                        self._adaptor.addChild(root_1, stream_notTerminal.nextTree())

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0


                elif alt55 == 2:
                    # /home/res/ANTLRv3_py.g:271:5: block
                    pass 
                    self._state.following.append(self.FOLLOW_block_in_notSet2015)
                    block113 = self.block()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_block.add(block113.tree)

                    # AST Rewrite
                    # elements: 89, block
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 271:12: -> ^( '~' block )
                        # /home/res/ANTLRv3_py.g:271:15: ^( '~' block )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(stream_89.nextNode(), root_1)

                        self._adaptor.addChild(root_1, stream_block.nextTree())

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0






                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "notSet"

    class treeSpec_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.treeSpec_return, self).__init__()

            self.tree = None




    # $ANTLR start "treeSpec"
    # /home/res/ANTLRv3_py.g:275:1: treeSpec : '^(' element ( element )+ ')' -> ^( TREE_BEGIN ( element )+ ) ;
    def treeSpec(self, ):

        retval = self.treeSpec_return()
        retval.start = self.input.LT(1)

        root_0 = None

        string_literal114 = None
        char_literal117 = None
        element115 = None

        element116 = None


        string_literal114_tree = None
        char_literal117_tree = None
        stream_TREE_BEGIN = RewriteRuleTokenStream(self._adaptor, "token TREE_BEGIN")
        stream_84 = RewriteRuleTokenStream(self._adaptor, "token 84")
        stream_element = RewriteRuleSubtreeStream(self._adaptor, "rule element")
        try:
            try:
                # /home/res/ANTLRv3_py.g:276:2: ( '^(' element ( element )+ ')' -> ^( TREE_BEGIN ( element )+ ) )
                # /home/res/ANTLRv3_py.g:276:4: '^(' element ( element )+ ')'
                pass 
                string_literal114=self.match(self.input, TREE_BEGIN, self.FOLLOW_TREE_BEGIN_in_treeSpec2039) 
                if self._state.backtracking == 0:
                    stream_TREE_BEGIN.add(string_literal114)
                self._state.following.append(self.FOLLOW_element_in_treeSpec2041)
                element115 = self.element()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_element.add(element115.tree)
                # /home/res/ANTLRv3_py.g:276:17: ( element )+
                cnt56 = 0
                while True: #loop56
                    alt56 = 2
                    LA56_0 = self.input.LA(1)

                    if (LA56_0 == SEMPRED or LA56_0 == TREE_BEGIN or (TOKEN_REF <= LA56_0 <= ACTION) or LA56_0 == RULE_REF or LA56_0 == 82 or LA56_0 == 89 or LA56_0 == 92) :
                        alt56 = 1


                    if alt56 == 1:
                        # /home/res/ANTLRv3_py.g:276:19: element
                        pass 
                        self._state.following.append(self.FOLLOW_element_in_treeSpec2045)
                        element116 = self.element()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_element.add(element116.tree)


                    else:
                        if cnt56 >= 1:
                            break #loop56

                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        eee = EarlyExitException(56, self.input)
                        raise eee

                    cnt56 += 1
                char_literal117=self.match(self.input, 84, self.FOLLOW_84_in_treeSpec2050) 
                if self._state.backtracking == 0:
                    stream_84.add(char_literal117)

                # AST Rewrite
                # elements: element
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 276:34: -> ^( TREE_BEGIN ( element )+ )
                    # /home/res/ANTLRv3_py.g:276:37: ^( TREE_BEGIN ( element )+ )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(self._adaptor.createFromType(TREE_BEGIN, "TREE_BEGIN"), root_1)

                    # /home/res/ANTLRv3_py.g:276:50: ( element )+
                    if not (stream_element.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_element.hasNext():
                        self._adaptor.addChild(root_1, stream_element.nextTree())


                    stream_element.reset()

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "treeSpec"

    class ebnf_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.ebnf_return, self).__init__()

            self.tree = None




    # $ANTLR start "ebnf"
    # /home/res/ANTLRv3_py.g:279:1: ebnf : block (op= '?' -> ^( OPTIONAL[op] block ) | op= '*' -> ^( CLOSURE[op] block ) | op= '+' -> ^( POSITIVE_CLOSURE[op] block ) | '=>' -> {self.gtype==COMBINED_GRAMMAR and \\\n\t\t\t\t\t Character.isUpperCase($rule::name.charAt(0))}? ^( SYNPRED[\"=>\"] block ) -> SYN_SEMPRED | -> block ) ;
    def ebnf(self, ):

        retval = self.ebnf_return()
        retval.start = self.input.LT(1)

        root_0 = None

        op = None
        string_literal119 = None
        block118 = None


        op_tree = None
        string_literal119_tree = None
        stream_91 = RewriteRuleTokenStream(self._adaptor, "token 91")
        stream_90 = RewriteRuleTokenStream(self._adaptor, "token 90")
        stream_74 = RewriteRuleTokenStream(self._adaptor, "token 74")
        stream_88 = RewriteRuleTokenStream(self._adaptor, "token 88")
        stream_block = RewriteRuleSubtreeStream(self._adaptor, "rule block")
               
        #    Token firstToken = input.LT(1);
        firstToken = self.input.LT(1)

        try:
            try:
                # /home/res/ANTLRv3_py.g:289:2: ( block (op= '?' -> ^( OPTIONAL[op] block ) | op= '*' -> ^( CLOSURE[op] block ) | op= '+' -> ^( POSITIVE_CLOSURE[op] block ) | '=>' -> {self.gtype==COMBINED_GRAMMAR and \\\n\t\t\t\t\t Character.isUpperCase($rule::name.charAt(0))}? ^( SYNPRED[\"=>\"] block ) -> SYN_SEMPRED | -> block ) )
                # /home/res/ANTLRv3_py.g:289:4: block (op= '?' -> ^( OPTIONAL[op] block ) | op= '*' -> ^( CLOSURE[op] block ) | op= '+' -> ^( POSITIVE_CLOSURE[op] block ) | '=>' -> {self.gtype==COMBINED_GRAMMAR and \\\n\t\t\t\t\t Character.isUpperCase($rule::name.charAt(0))}? ^( SYNPRED[\"=>\"] block ) -> SYN_SEMPRED | -> block )
                pass 
                self._state.following.append(self.FOLLOW_block_in_ebnf2082)
                block118 = self.block()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_block.add(block118.tree)
                # /home/res/ANTLRv3_py.g:290:3: (op= '?' -> ^( OPTIONAL[op] block ) | op= '*' -> ^( CLOSURE[op] block ) | op= '+' -> ^( POSITIVE_CLOSURE[op] block ) | '=>' -> {self.gtype==COMBINED_GRAMMAR and \\\n\t\t\t\t\t Character.isUpperCase($rule::name.charAt(0))}? ^( SYNPRED[\"=>\"] block ) -> SYN_SEMPRED | -> block )
                alt57 = 5
                LA57 = self.input.LA(1)
                if LA57 == 90:
                    alt57 = 1
                elif LA57 == 74:
                    alt57 = 2
                elif LA57 == 91:
                    alt57 = 3
                elif LA57 == 88:
                    alt57 = 4
                elif LA57 == SEMPRED or LA57 == TREE_BEGIN or LA57 == REWRITE or LA57 == TOKEN_REF or LA57 == STRING_LITERAL or LA57 == CHAR_LITERAL or LA57 == ACTION or LA57 == RULE_REF or LA57 == 69 or LA57 == 82 or LA57 == 83 or LA57 == 84 or LA57 == 89 or LA57 == 92:
                    alt57 = 5
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed

                    nvae = NoViableAltException("", 57, 0, self.input)

                    raise nvae

                if alt57 == 1:
                    # /home/res/ANTLRv3_py.g:290:5: op= '?'
                    pass 
                    op=self.match(self.input, 90, self.FOLLOW_90_in_ebnf2090) 
                    if self._state.backtracking == 0:
                        stream_90.add(op)

                    # AST Rewrite
                    # elements: block
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 290:12: -> ^( OPTIONAL[op] block )
                        # /home/res/ANTLRv3_py.g:290:15: ^( OPTIONAL[op] block )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(self._adaptor.create(OPTIONAL, op), root_1)

                        self._adaptor.addChild(root_1, stream_block.nextTree())

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0


                elif alt57 == 2:
                    # /home/res/ANTLRv3_py.g:291:5: op= '*'
                    pass 
                    op=self.match(self.input, 74, self.FOLLOW_74_in_ebnf2107) 
                    if self._state.backtracking == 0:
                        stream_74.add(op)

                    # AST Rewrite
                    # elements: block
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 291:12: -> ^( CLOSURE[op] block )
                        # /home/res/ANTLRv3_py.g:291:15: ^( CLOSURE[op] block )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(self._adaptor.create(CLOSURE, op), root_1)

                        self._adaptor.addChild(root_1, stream_block.nextTree())

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0


                elif alt57 == 3:
                    # /home/res/ANTLRv3_py.g:292:5: op= '+'
                    pass 
                    op=self.match(self.input, 91, self.FOLLOW_91_in_ebnf2124) 
                    if self._state.backtracking == 0:
                        stream_91.add(op)

                    # AST Rewrite
                    # elements: block
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 292:12: -> ^( POSITIVE_CLOSURE[op] block )
                        # /home/res/ANTLRv3_py.g:292:15: ^( POSITIVE_CLOSURE[op] block )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(self._adaptor.create(POSITIVE_CLOSURE, op), root_1)

                        self._adaptor.addChild(root_1, stream_block.nextTree())

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0


                elif alt57 == 4:
                    # /home/res/ANTLRv3_py.g:293:7: '=>'
                    pass 
                    string_literal119=self.match(self.input, 88, self.FOLLOW_88_in_ebnf2141) 
                    if self._state.backtracking == 0:
                        stream_88.add(string_literal119)

                    # AST Rewrite
                    # elements: block
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        if self.gtype==COMBINED_GRAMMAR and \
                        self.rule_stack[-1].name[0:1].isupper():
                            # 294:6: -> {self.gtype==COMBINED_GRAMMAR and \\\n\t\t\t\t\t Character.isUpperCase($rule::name.charAt(0))}? ^( SYNPRED[\"=>\"] block )
                            # /home/res/ANTLRv3_py.g:296:9: ^( SYNPRED[\"=>\"] block )
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(self._adaptor.create(SYNPRED, "=>"), root_1)

                            self._adaptor.addChild(root_1, stream_block.nextTree())

                            self._adaptor.addChild(root_0, root_1)


                        else: 
                            # 298:6: -> SYN_SEMPRED
                            self._adaptor.addChild(root_0, self._adaptor.createFromType(SYN_SEMPRED, "SYN_SEMPRED"))


                        retval.tree = root_0


                elif alt57 == 5:
                    # /home/res/ANTLRv3_py.g:299:13: 
                    pass 
                    # AST Rewrite
                    # elements: block
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 299:13: -> block
                        self._adaptor.addChild(root_0, stream_block.nextTree())



                        retval.tree = root_0






                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)

                if self._state.backtracking == 0:
                            
                    retval.tree.getToken().setLine(firstToken.getLine());
                    retval.tree.getToken().setCharPositionInLine(firstToken.getCharPositionInLine());



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "ebnf"

    class range_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.range_return, self).__init__()

            self.tree = None




    # $ANTLR start "range"
    # /home/res/ANTLRv3_py.g:303:1: range : c1= CHAR_LITERAL RANGE c2= CHAR_LITERAL -> ^( CHAR_RANGE[$c1,\"..\"] $c1 $c2) ;
    def range(self, ):

        retval = self.range_return()
        retval.start = self.input.LT(1)

        root_0 = None

        c1 = None
        c2 = None
        RANGE120 = None

        c1_tree = None
        c2_tree = None
        RANGE120_tree = None
        stream_RANGE = RewriteRuleTokenStream(self._adaptor, "token RANGE")
        stream_CHAR_LITERAL = RewriteRuleTokenStream(self._adaptor, "token CHAR_LITERAL")

        try:
            try:
                # /home/res/ANTLRv3_py.g:304:2: (c1= CHAR_LITERAL RANGE c2= CHAR_LITERAL -> ^( CHAR_RANGE[$c1,\"..\"] $c1 $c2) )
                # /home/res/ANTLRv3_py.g:304:4: c1= CHAR_LITERAL RANGE c2= CHAR_LITERAL
                pass 
                c1=self.match(self.input, CHAR_LITERAL, self.FOLLOW_CHAR_LITERAL_in_range2224) 
                if self._state.backtracking == 0:
                    stream_CHAR_LITERAL.add(c1)
                RANGE120=self.match(self.input, RANGE, self.FOLLOW_RANGE_in_range2226) 
                if self._state.backtracking == 0:
                    stream_RANGE.add(RANGE120)
                c2=self.match(self.input, CHAR_LITERAL, self.FOLLOW_CHAR_LITERAL_in_range2230) 
                if self._state.backtracking == 0:
                    stream_CHAR_LITERAL.add(c2)

                # AST Rewrite
                # elements: c2, c1
                # token labels: c1, c2
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0
                    stream_c1 = RewriteRuleTokenStream(self._adaptor, "token c1", c1)
                    stream_c2 = RewriteRuleTokenStream(self._adaptor, "token c2", c2)

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 304:42: -> ^( CHAR_RANGE[$c1,\"..\"] $c1 $c2)
                    # /home/res/ANTLRv3_py.g:304:45: ^( CHAR_RANGE[$c1,\"..\"] $c1 $c2)
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(self._adaptor.create(CHAR_RANGE, c1, ".."), root_1)

                    self._adaptor.addChild(root_1, stream_c1.nextNode())
                    self._adaptor.addChild(root_1, stream_c2.nextNode())

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "range"

    class terminal_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.terminal_return, self).__init__()

            self.tree = None




    # $ANTLR start "terminal"
    # /home/res/ANTLRv3_py.g:307:1: terminal : ( CHAR_LITERAL -> CHAR_LITERAL | TOKEN_REF ( ARG_ACTION -> ^( TOKEN_REF ARG_ACTION ) | -> TOKEN_REF ) | STRING_LITERAL -> STRING_LITERAL | '.' -> '.' ) ( '^' -> ^( '^' $terminal) | '!' -> ^( '!' $terminal) )? ;
    def terminal(self, ):

        retval = self.terminal_return()
        retval.start = self.input.LT(1)

        root_0 = None

        CHAR_LITERAL121 = None
        TOKEN_REF122 = None
        ARG_ACTION123 = None
        STRING_LITERAL124 = None
        char_literal125 = None
        char_literal126 = None
        char_literal127 = None

        CHAR_LITERAL121_tree = None
        TOKEN_REF122_tree = None
        ARG_ACTION123_tree = None
        STRING_LITERAL124_tree = None
        char_literal125_tree = None
        char_literal126_tree = None
        char_literal127_tree = None
        stream_STRING_LITERAL = RewriteRuleTokenStream(self._adaptor, "token STRING_LITERAL")
        stream_BANG = RewriteRuleTokenStream(self._adaptor, "token BANG")
        stream_92 = RewriteRuleTokenStream(self._adaptor, "token 92")
        stream_CHAR_LITERAL = RewriteRuleTokenStream(self._adaptor, "token CHAR_LITERAL")
        stream_ROOT = RewriteRuleTokenStream(self._adaptor, "token ROOT")
        stream_TOKEN_REF = RewriteRuleTokenStream(self._adaptor, "token TOKEN_REF")
        stream_ARG_ACTION = RewriteRuleTokenStream(self._adaptor, "token ARG_ACTION")

        try:
            try:
                # /home/res/ANTLRv3_py.g:308:5: ( ( CHAR_LITERAL -> CHAR_LITERAL | TOKEN_REF ( ARG_ACTION -> ^( TOKEN_REF ARG_ACTION ) | -> TOKEN_REF ) | STRING_LITERAL -> STRING_LITERAL | '.' -> '.' ) ( '^' -> ^( '^' $terminal) | '!' -> ^( '!' $terminal) )? )
                # /home/res/ANTLRv3_py.g:308:9: ( CHAR_LITERAL -> CHAR_LITERAL | TOKEN_REF ( ARG_ACTION -> ^( TOKEN_REF ARG_ACTION ) | -> TOKEN_REF ) | STRING_LITERAL -> STRING_LITERAL | '.' -> '.' ) ( '^' -> ^( '^' $terminal) | '!' -> ^( '!' $terminal) )?
                pass 
                # /home/res/ANTLRv3_py.g:308:9: ( CHAR_LITERAL -> CHAR_LITERAL | TOKEN_REF ( ARG_ACTION -> ^( TOKEN_REF ARG_ACTION ) | -> TOKEN_REF ) | STRING_LITERAL -> STRING_LITERAL | '.' -> '.' )
                alt59 = 4
                LA59 = self.input.LA(1)
                if LA59 == CHAR_LITERAL:
                    alt59 = 1
                elif LA59 == TOKEN_REF:
                    alt59 = 2
                elif LA59 == STRING_LITERAL:
                    alt59 = 3
                elif LA59 == 92:
                    alt59 = 4
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed

                    nvae = NoViableAltException("", 59, 0, self.input)

                    raise nvae

                if alt59 == 1:
                    # /home/res/ANTLRv3_py.g:308:11: CHAR_LITERAL
                    pass 
                    CHAR_LITERAL121=self.match(self.input, CHAR_LITERAL, self.FOLLOW_CHAR_LITERAL_in_terminal2261) 
                    if self._state.backtracking == 0:
                        stream_CHAR_LITERAL.add(CHAR_LITERAL121)

                    # AST Rewrite
                    # elements: CHAR_LITERAL
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 308:27: -> CHAR_LITERAL
                        self._adaptor.addChild(root_0, stream_CHAR_LITERAL.nextNode())



                        retval.tree = root_0


                elif alt59 == 2:
                    # /home/res/ANTLRv3_py.g:310:7: TOKEN_REF ( ARG_ACTION -> ^( TOKEN_REF ARG_ACTION ) | -> TOKEN_REF )
                    pass 
                    TOKEN_REF122=self.match(self.input, TOKEN_REF, self.FOLLOW_TOKEN_REF_in_terminal2283) 
                    if self._state.backtracking == 0:
                        stream_TOKEN_REF.add(TOKEN_REF122)
                    # /home/res/ANTLRv3_py.g:311:4: ( ARG_ACTION -> ^( TOKEN_REF ARG_ACTION ) | -> TOKEN_REF )
                    alt58 = 2
                    LA58_0 = self.input.LA(1)

                    if (LA58_0 == ARG_ACTION) :
                        alt58 = 1
                    elif (LA58_0 == SEMPRED or (TREE_BEGIN <= LA58_0 <= REWRITE) or (TOKEN_REF <= LA58_0 <= ACTION) or LA58_0 == RULE_REF or LA58_0 == 69 or LA58_0 == 74 or (82 <= LA58_0 <= 84) or (89 <= LA58_0 <= 92)) :
                        alt58 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        nvae = NoViableAltException("", 58, 0, self.input)

                        raise nvae

                    if alt58 == 1:
                        # /home/res/ANTLRv3_py.g:311:6: ARG_ACTION
                        pass 
                        ARG_ACTION123=self.match(self.input, ARG_ACTION, self.FOLLOW_ARG_ACTION_in_terminal2290) 
                        if self._state.backtracking == 0:
                            stream_ARG_ACTION.add(ARG_ACTION123)

                        # AST Rewrite
                        # elements: ARG_ACTION, TOKEN_REF
                        # token labels: 
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 311:20: -> ^( TOKEN_REF ARG_ACTION )
                            # /home/res/ANTLRv3_py.g:311:23: ^( TOKEN_REF ARG_ACTION )
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(stream_TOKEN_REF.nextNode(), root_1)

                            self._adaptor.addChild(root_1, stream_ARG_ACTION.nextNode())

                            self._adaptor.addChild(root_0, root_1)



                            retval.tree = root_0


                    elif alt58 == 2:
                        # /home/res/ANTLRv3_py.g:312:12: 
                        pass 
                        # AST Rewrite
                        # elements: TOKEN_REF
                        # token labels: 
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 312:12: -> TOKEN_REF
                            self._adaptor.addChild(root_0, stream_TOKEN_REF.nextNode())



                            retval.tree = root_0





                elif alt59 == 3:
                    # /home/res/ANTLRv3_py.g:314:7: STRING_LITERAL
                    pass 
                    STRING_LITERAL124=self.match(self.input, STRING_LITERAL, self.FOLLOW_STRING_LITERAL_in_terminal2329) 
                    if self._state.backtracking == 0:
                        stream_STRING_LITERAL.add(STRING_LITERAL124)

                    # AST Rewrite
                    # elements: STRING_LITERAL
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 314:25: -> STRING_LITERAL
                        self._adaptor.addChild(root_0, stream_STRING_LITERAL.nextNode())



                        retval.tree = root_0


                elif alt59 == 4:
                    # /home/res/ANTLRv3_py.g:315:7: '.'
                    pass 
                    char_literal125=self.match(self.input, 92, self.FOLLOW_92_in_terminal2344) 
                    if self._state.backtracking == 0:
                        stream_92.add(char_literal125)

                    # AST Rewrite
                    # elements: 92
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 315:17: -> '.'
                        self._adaptor.addChild(root_0, stream_92.nextNode())



                        retval.tree = root_0



                # /home/res/ANTLRv3_py.g:317:3: ( '^' -> ^( '^' $terminal) | '!' -> ^( '!' $terminal) )?
                alt60 = 3
                LA60_0 = self.input.LA(1)

                if (LA60_0 == ROOT) :
                    alt60 = 1
                elif (LA60_0 == BANG) :
                    alt60 = 2
                if alt60 == 1:
                    # /home/res/ANTLRv3_py.g:317:5: '^'
                    pass 
                    char_literal126=self.match(self.input, ROOT, self.FOLLOW_ROOT_in_terminal2365) 
                    if self._state.backtracking == 0:
                        stream_ROOT.add(char_literal126)

                    # AST Rewrite
                    # elements: terminal, ROOT
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 317:15: -> ^( '^' $terminal)
                        # /home/res/ANTLRv3_py.g:317:18: ^( '^' $terminal)
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(stream_ROOT.nextNode(), root_1)

                        self._adaptor.addChild(root_1, stream_retval.nextTree())

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0


                elif alt60 == 2:
                    # /home/res/ANTLRv3_py.g:318:5: '!'
                    pass 
                    char_literal127=self.match(self.input, BANG, self.FOLLOW_BANG_in_terminal2386) 
                    if self._state.backtracking == 0:
                        stream_BANG.add(char_literal127)

                    # AST Rewrite
                    # elements: BANG, terminal
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 318:15: -> ^( '!' $terminal)
                        # /home/res/ANTLRv3_py.g:318:18: ^( '!' $terminal)
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(stream_BANG.nextNode(), root_1)

                        self._adaptor.addChild(root_1, stream_retval.nextTree())

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0






                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "terminal"

    class notTerminal_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.notTerminal_return, self).__init__()

            self.tree = None




    # $ANTLR start "notTerminal"
    # /home/res/ANTLRv3_py.g:322:1: notTerminal : ( CHAR_LITERAL | TOKEN_REF | STRING_LITERAL );
    def notTerminal(self, ):

        retval = self.notTerminal_return()
        retval.start = self.input.LT(1)

        root_0 = None

        set128 = None

        set128_tree = None

        try:
            try:
                # /home/res/ANTLRv3_py.g:323:2: ( CHAR_LITERAL | TOKEN_REF | STRING_LITERAL )
                # /home/res/ANTLRv3_py.g:
                pass 
                root_0 = self._adaptor.nil()

                set128 = self.input.LT(1)
                if (TOKEN_REF <= self.input.LA(1) <= CHAR_LITERAL):
                    self.input.consume()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, self._adaptor.createWithPayload(set128))
                    self._state.errorRecovery = False

                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed

                    mse = MismatchedSetException(None, self.input)
                    raise mse





                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "notTerminal"

    class ebnfSuffix_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.ebnfSuffix_return, self).__init__()

            self.tree = None




    # $ANTLR start "ebnfSuffix"
    # /home/res/ANTLRv3_py.g:328:1: ebnfSuffix : ( '?' -> OPTIONAL[op] | '*' -> CLOSURE[op] | '+' -> POSITIVE_CLOSURE[op] );
    def ebnfSuffix(self, ):

        retval = self.ebnfSuffix_return()
        retval.start = self.input.LT(1)

        root_0 = None

        char_literal129 = None
        char_literal130 = None
        char_literal131 = None

        char_literal129_tree = None
        char_literal130_tree = None
        char_literal131_tree = None
        stream_91 = RewriteRuleTokenStream(self._adaptor, "token 91")
        stream_90 = RewriteRuleTokenStream(self._adaptor, "token 90")
        stream_74 = RewriteRuleTokenStream(self._adaptor, "token 74")

               
        #	Token op = input.LT(1);
        op = self.input.LT(1)

        try:
            try:
                # /home/res/ANTLRv3_py.g:333:2: ( '?' -> OPTIONAL[op] | '*' -> CLOSURE[op] | '+' -> POSITIVE_CLOSURE[op] )
                alt61 = 3
                LA61 = self.input.LA(1)
                if LA61 == 90:
                    alt61 = 1
                elif LA61 == 74:
                    alt61 = 2
                elif LA61 == 91:
                    alt61 = 3
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed

                    nvae = NoViableAltException("", 61, 0, self.input)

                    raise nvae

                if alt61 == 1:
                    # /home/res/ANTLRv3_py.g:333:4: '?'
                    pass 
                    char_literal129=self.match(self.input, 90, self.FOLLOW_90_in_ebnfSuffix2446) 
                    if self._state.backtracking == 0:
                        stream_90.add(char_literal129)

                    # AST Rewrite
                    # elements: 
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 333:8: -> OPTIONAL[op]
                        self._adaptor.addChild(root_0, self._adaptor.create(OPTIONAL, op))



                        retval.tree = root_0


                elif alt61 == 2:
                    # /home/res/ANTLRv3_py.g:334:6: '*'
                    pass 
                    char_literal130=self.match(self.input, 74, self.FOLLOW_74_in_ebnfSuffix2458) 
                    if self._state.backtracking == 0:
                        stream_74.add(char_literal130)

                    # AST Rewrite
                    # elements: 
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 334:10: -> CLOSURE[op]
                        self._adaptor.addChild(root_0, self._adaptor.create(CLOSURE, op))



                        retval.tree = root_0


                elif alt61 == 3:
                    # /home/res/ANTLRv3_py.g:335:7: '+'
                    pass 
                    char_literal131=self.match(self.input, 91, self.FOLLOW_91_in_ebnfSuffix2471) 
                    if self._state.backtracking == 0:
                        stream_91.add(char_literal131)

                    # AST Rewrite
                    # elements: 
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 335:11: -> POSITIVE_CLOSURE[op]
                        self._adaptor.addChild(root_0, self._adaptor.create(POSITIVE_CLOSURE, op))



                        retval.tree = root_0


                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "ebnfSuffix"

    class rewrite_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.rewrite_return, self).__init__()

            self.tree = None




    # $ANTLR start "rewrite"
    # /home/res/ANTLRv3_py.g:342:1: rewrite : ( (rew+= '->' preds+= SEMPRED predicated+= rewrite_alternative )* rew2= '->' last= rewrite_alternative -> ( ^( $rew $preds $predicated) )* ^( $rew2 $last) | );
    def rewrite(self, ):

        retval = self.rewrite_return()
        retval.start = self.input.LT(1)

        root_0 = None

        rew2 = None
        rew = None
        preds = None
        list_rew = None
        list_preds = None
        list_predicated = None
        last = None

        predicated = None

        predicated = None
        rew2_tree = None
        rew_tree = None
        preds_tree = None
        stream_SEMPRED = RewriteRuleTokenStream(self._adaptor, "token SEMPRED")
        stream_REWRITE = RewriteRuleTokenStream(self._adaptor, "token REWRITE")
        stream_rewrite_alternative = RewriteRuleSubtreeStream(self._adaptor, "rule rewrite_alternative")
               
        #	Token firstToken = input.LT(1);
        firstToken = self.input.LT(1)

        try:
            try:
                # /home/res/ANTLRv3_py.g:347:2: ( (rew+= '->' preds+= SEMPRED predicated+= rewrite_alternative )* rew2= '->' last= rewrite_alternative -> ( ^( $rew $preds $predicated) )* ^( $rew2 $last) | )
                alt63 = 2
                LA63_0 = self.input.LA(1)

                if (LA63_0 == REWRITE) :
                    alt63 = 1
                elif (LA63_0 == 69 or (83 <= LA63_0 <= 84)) :
                    alt63 = 2
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed

                    nvae = NoViableAltException("", 63, 0, self.input)

                    raise nvae

                if alt63 == 1:
                    # /home/res/ANTLRv3_py.g:347:4: (rew+= '->' preds+= SEMPRED predicated+= rewrite_alternative )* rew2= '->' last= rewrite_alternative
                    pass 
                    # /home/res/ANTLRv3_py.g:347:4: (rew+= '->' preds+= SEMPRED predicated+= rewrite_alternative )*
                    while True: #loop62
                        alt62 = 2
                        LA62_0 = self.input.LA(1)

                        if (LA62_0 == REWRITE) :
                            LA62_1 = self.input.LA(2)

                            if (LA62_1 == SEMPRED) :
                                alt62 = 1




                        if alt62 == 1:
                            # /home/res/ANTLRv3_py.g:347:5: rew+= '->' preds+= SEMPRED predicated+= rewrite_alternative
                            pass 
                            rew=self.match(self.input, REWRITE, self.FOLLOW_REWRITE_in_rewrite2500) 
                            if self._state.backtracking == 0:
                                stream_REWRITE.add(rew)
                            if list_rew is None:
                                list_rew = []
                            list_rew.append(rew)

                            preds=self.match(self.input, SEMPRED, self.FOLLOW_SEMPRED_in_rewrite2504) 
                            if self._state.backtracking == 0:
                                stream_SEMPRED.add(preds)
                            if list_preds is None:
                                list_preds = []
                            list_preds.append(preds)

                            self._state.following.append(self.FOLLOW_rewrite_alternative_in_rewrite2508)
                            predicated = self.rewrite_alternative()

                            self._state.following.pop()
                            if self._state.backtracking == 0:
                                stream_rewrite_alternative.add(predicated.tree)
                            if list_predicated is None:
                                list_predicated = []
                            list_predicated.append(predicated.tree)



                        else:
                            break #loop62
                    rew2=self.match(self.input, REWRITE, self.FOLLOW_REWRITE_in_rewrite2516) 
                    if self._state.backtracking == 0:
                        stream_REWRITE.add(rew2)
                    self._state.following.append(self.FOLLOW_rewrite_alternative_in_rewrite2520)
                    last = self.rewrite_alternative()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_rewrite_alternative.add(last.tree)

                    # AST Rewrite
                    # elements: last, predicated, rew, rew2, preds
                    # token labels: rew2
                    # rule labels: retval, last
                    # token list labels: rew, preds
                    # rule list labels: predicated
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0
                        stream_rew2 = RewriteRuleTokenStream(self._adaptor, "token rew2", rew2)
                        stream_rew = RewriteRuleTokenStream(self._adaptor, "token rew", list_rew)
                        stream_preds = RewriteRuleTokenStream(self._adaptor, "token preds", list_preds)

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        if last is not None:
                            stream_last = RewriteRuleSubtreeStream(self._adaptor, "rule last", last.tree)
                        else:
                            stream_last = RewriteRuleSubtreeStream(self._adaptor, "token last", None)

                        stream_predicated = RewriteRuleSubtreeStream(self._adaptor, "token predicated", list_predicated)
                        root_0 = self._adaptor.nil()
                        # 349:9: -> ( ^( $rew $preds $predicated) )* ^( $rew2 $last)
                        # /home/res/ANTLRv3_py.g:349:12: ( ^( $rew $preds $predicated) )*
                        while stream_predicated.hasNext() or stream_rew.hasNext() or stream_preds.hasNext():
                            # /home/res/ANTLRv3_py.g:349:12: ^( $rew $preds $predicated)
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(stream_rew.nextNode(), root_1)

                            self._adaptor.addChild(root_1, stream_preds.nextNode())
                            self._adaptor.addChild(root_1, stream_predicated.nextTree())

                            self._adaptor.addChild(root_0, root_1)


                        stream_predicated.reset();
                        stream_rew.reset();
                        stream_preds.reset();
                        # /home/res/ANTLRv3_py.g:349:40: ^( $rew2 $last)
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(stream_rew2.nextNode(), root_1)

                        self._adaptor.addChild(root_1, stream_last.nextTree())

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0


                elif alt63 == 2:
                    # /home/res/ANTLRv3_py.g:351:2: 
                    pass 
                    root_0 = self._adaptor.nil()


                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "rewrite"

    class rewrite_alternative_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.rewrite_alternative_return, self).__init__()

            self.tree = None




    # $ANTLR start "rewrite_alternative"
    # /home/res/ANTLRv3_py.g:353:1: rewrite_alternative options {backtrack=true; } : ( rewrite_template | rewrite_tree_alternative | -> ^( ALT[\"ALT\"] EPSILON[\"EPSILON\"] EOA[\"EOA\"] ) );
    def rewrite_alternative(self, ):

        retval = self.rewrite_alternative_return()
        retval.start = self.input.LT(1)

        root_0 = None

        rewrite_template132 = None

        rewrite_tree_alternative133 = None



        try:
            try:
                # /home/res/ANTLRv3_py.g:355:2: ( rewrite_template | rewrite_tree_alternative | -> ^( ALT[\"ALT\"] EPSILON[\"EPSILON\"] EOA[\"EOA\"] ) )
                alt64 = 3
                alt64 = self.dfa64.predict(self.input)
                if alt64 == 1:
                    # /home/res/ANTLRv3_py.g:355:4: rewrite_template
                    pass 
                    root_0 = self._adaptor.nil()

                    self._state.following.append(self.FOLLOW_rewrite_template_in_rewrite_alternative2571)
                    rewrite_template132 = self.rewrite_template()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, rewrite_template132.tree)


                elif alt64 == 2:
                    # /home/res/ANTLRv3_py.g:356:4: rewrite_tree_alternative
                    pass 
                    root_0 = self._adaptor.nil()

                    self._state.following.append(self.FOLLOW_rewrite_tree_alternative_in_rewrite_alternative2576)
                    rewrite_tree_alternative133 = self.rewrite_tree_alternative()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, rewrite_tree_alternative133.tree)


                elif alt64 == 3:
                    # /home/res/ANTLRv3_py.g:357:29: 
                    pass 
                    # AST Rewrite
                    # elements: 
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 357:29: -> ^( ALT[\"ALT\"] EPSILON[\"EPSILON\"] EOA[\"EOA\"] )
                        # /home/res/ANTLRv3_py.g:357:32: ^( ALT[\"ALT\"] EPSILON[\"EPSILON\"] EOA[\"EOA\"] )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(self._adaptor.create(ALT, "ALT"), root_1)

                        self._adaptor.addChild(root_1, self._adaptor.create(EPSILON, "EPSILON"))
                        self._adaptor.addChild(root_1, self._adaptor.create(EOA, "EOA"))

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0


                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "rewrite_alternative"

    class rewrite_tree_block_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.rewrite_tree_block_return, self).__init__()

            self.tree = None




    # $ANTLR start "rewrite_tree_block"
    # /home/res/ANTLRv3_py.g:360:1: rewrite_tree_block : lp= '(' rewrite_tree_alternative ')' -> ^( BLOCK[$lp,\"BLOCK\"] rewrite_tree_alternative EOB[$lp,\"EOB\"] ) ;
    def rewrite_tree_block(self, ):

        retval = self.rewrite_tree_block_return()
        retval.start = self.input.LT(1)

        root_0 = None

        lp = None
        char_literal135 = None
        rewrite_tree_alternative134 = None


        lp_tree = None
        char_literal135_tree = None
        stream_82 = RewriteRuleTokenStream(self._adaptor, "token 82")
        stream_84 = RewriteRuleTokenStream(self._adaptor, "token 84")
        stream_rewrite_tree_alternative = RewriteRuleSubtreeStream(self._adaptor, "rule rewrite_tree_alternative")
        try:
            try:
                # /home/res/ANTLRv3_py.g:361:5: (lp= '(' rewrite_tree_alternative ')' -> ^( BLOCK[$lp,\"BLOCK\"] rewrite_tree_alternative EOB[$lp,\"EOB\"] ) )
                # /home/res/ANTLRv3_py.g:361:9: lp= '(' rewrite_tree_alternative ')'
                pass 
                lp=self.match(self.input, 82, self.FOLLOW_82_in_rewrite_tree_block2618) 
                if self._state.backtracking == 0:
                    stream_82.add(lp)
                self._state.following.append(self.FOLLOW_rewrite_tree_alternative_in_rewrite_tree_block2620)
                rewrite_tree_alternative134 = self.rewrite_tree_alternative()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_rewrite_tree_alternative.add(rewrite_tree_alternative134.tree)
                char_literal135=self.match(self.input, 84, self.FOLLOW_84_in_rewrite_tree_block2622) 
                if self._state.backtracking == 0:
                    stream_84.add(char_literal135)

                # AST Rewrite
                # elements: rewrite_tree_alternative
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 362:6: -> ^( BLOCK[$lp,\"BLOCK\"] rewrite_tree_alternative EOB[$lp,\"EOB\"] )
                    # /home/res/ANTLRv3_py.g:362:9: ^( BLOCK[$lp,\"BLOCK\"] rewrite_tree_alternative EOB[$lp,\"EOB\"] )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(self._adaptor.create(BLOCK, lp, "BLOCK"), root_1)

                    self._adaptor.addChild(root_1, stream_rewrite_tree_alternative.nextTree())
                    self._adaptor.addChild(root_1, self._adaptor.create(EOB, lp, "EOB"))

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "rewrite_tree_block"

    class rewrite_tree_alternative_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.rewrite_tree_alternative_return, self).__init__()

            self.tree = None




    # $ANTLR start "rewrite_tree_alternative"
    # /home/res/ANTLRv3_py.g:365:1: rewrite_tree_alternative : ( rewrite_tree_element )+ -> ^( ALT[\"ALT\"] ( rewrite_tree_element )+ EOA[\"EOA\"] ) ;
    def rewrite_tree_alternative(self, ):

        retval = self.rewrite_tree_alternative_return()
        retval.start = self.input.LT(1)

        root_0 = None

        rewrite_tree_element136 = None


        stream_rewrite_tree_element = RewriteRuleSubtreeStream(self._adaptor, "rule rewrite_tree_element")
        try:
            try:
                # /home/res/ANTLRv3_py.g:366:5: ( ( rewrite_tree_element )+ -> ^( ALT[\"ALT\"] ( rewrite_tree_element )+ EOA[\"EOA\"] ) )
                # /home/res/ANTLRv3_py.g:366:7: ( rewrite_tree_element )+
                pass 
                # /home/res/ANTLRv3_py.g:366:7: ( rewrite_tree_element )+
                cnt65 = 0
                while True: #loop65
                    alt65 = 2
                    LA65_0 = self.input.LA(1)

                    if (LA65_0 == TREE_BEGIN or (TOKEN_REF <= LA65_0 <= ACTION) or LA65_0 == RULE_REF or LA65_0 == 82 or LA65_0 == 93) :
                        alt65 = 1


                    if alt65 == 1:
                        # /home/res/ANTLRv3_py.g:366:7: rewrite_tree_element
                        pass 
                        self._state.following.append(self.FOLLOW_rewrite_tree_element_in_rewrite_tree_alternative2656)
                        rewrite_tree_element136 = self.rewrite_tree_element()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_rewrite_tree_element.add(rewrite_tree_element136.tree)


                    else:
                        if cnt65 >= 1:
                            break #loop65

                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        eee = EarlyExitException(65, self.input)
                        raise eee

                    cnt65 += 1

                # AST Rewrite
                # elements: rewrite_tree_element
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 366:29: -> ^( ALT[\"ALT\"] ( rewrite_tree_element )+ EOA[\"EOA\"] )
                    # /home/res/ANTLRv3_py.g:366:32: ^( ALT[\"ALT\"] ( rewrite_tree_element )+ EOA[\"EOA\"] )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(self._adaptor.create(ALT, "ALT"), root_1)

                    # /home/res/ANTLRv3_py.g:366:45: ( rewrite_tree_element )+
                    if not (stream_rewrite_tree_element.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_rewrite_tree_element.hasNext():
                        self._adaptor.addChild(root_1, stream_rewrite_tree_element.nextTree())


                    stream_rewrite_tree_element.reset()
                    self._adaptor.addChild(root_1, self._adaptor.create(EOA, "EOA"))

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "rewrite_tree_alternative"

    class rewrite_tree_element_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.rewrite_tree_element_return, self).__init__()

            self.tree = None




    # $ANTLR start "rewrite_tree_element"
    # /home/res/ANTLRv3_py.g:369:1: rewrite_tree_element : ( rewrite_tree_atom | rewrite_tree_atom ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] rewrite_tree_atom EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | rewrite_tree ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] rewrite_tree EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> rewrite_tree ) | rewrite_tree_ebnf );
    def rewrite_tree_element(self, ):

        retval = self.rewrite_tree_element_return()
        retval.start = self.input.LT(1)

        root_0 = None

        rewrite_tree_atom137 = None

        rewrite_tree_atom138 = None

        ebnfSuffix139 = None

        rewrite_tree140 = None

        ebnfSuffix141 = None

        rewrite_tree_ebnf142 = None


        stream_rewrite_tree = RewriteRuleSubtreeStream(self._adaptor, "rule rewrite_tree")
        stream_ebnfSuffix = RewriteRuleSubtreeStream(self._adaptor, "rule ebnfSuffix")
        stream_rewrite_tree_atom = RewriteRuleSubtreeStream(self._adaptor, "rule rewrite_tree_atom")
        try:
            try:
                # /home/res/ANTLRv3_py.g:370:2: ( rewrite_tree_atom | rewrite_tree_atom ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] rewrite_tree_atom EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | rewrite_tree ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] rewrite_tree EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> rewrite_tree ) | rewrite_tree_ebnf )
                alt67 = 4
                alt67 = self.dfa67.predict(self.input)
                if alt67 == 1:
                    # /home/res/ANTLRv3_py.g:370:4: rewrite_tree_atom
                    pass 
                    root_0 = self._adaptor.nil()

                    self._state.following.append(self.FOLLOW_rewrite_tree_atom_in_rewrite_tree_element2684)
                    rewrite_tree_atom137 = self.rewrite_tree_atom()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, rewrite_tree_atom137.tree)


                elif alt67 == 2:
                    # /home/res/ANTLRv3_py.g:371:4: rewrite_tree_atom ebnfSuffix
                    pass 
                    self._state.following.append(self.FOLLOW_rewrite_tree_atom_in_rewrite_tree_element2689)
                    rewrite_tree_atom138 = self.rewrite_tree_atom()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_rewrite_tree_atom.add(rewrite_tree_atom138.tree)
                    self._state.following.append(self.FOLLOW_ebnfSuffix_in_rewrite_tree_element2691)
                    ebnfSuffix139 = self.ebnfSuffix()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_ebnfSuffix.add(ebnfSuffix139.tree)

                    # AST Rewrite
                    # elements: rewrite_tree_atom, ebnfSuffix
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 372:3: -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] rewrite_tree_atom EOA[\"EOA\"] ) EOB[\"EOB\"] ) )
                        # /home/res/ANTLRv3_py.g:372:6: ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] rewrite_tree_atom EOA[\"EOA\"] ) EOB[\"EOB\"] ) )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(stream_ebnfSuffix.nextNode(), root_1)

                        # /home/res/ANTLRv3_py.g:372:20: ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] rewrite_tree_atom EOA[\"EOA\"] ) EOB[\"EOB\"] )
                        root_2 = self._adaptor.nil()
                        root_2 = self._adaptor.becomeRoot(self._adaptor.create(BLOCK, "BLOCK"), root_2)

                        # /home/res/ANTLRv3_py.g:372:37: ^( ALT[\"ALT\"] rewrite_tree_atom EOA[\"EOA\"] )
                        root_3 = self._adaptor.nil()
                        root_3 = self._adaptor.becomeRoot(self._adaptor.create(ALT, "ALT"), root_3)

                        self._adaptor.addChild(root_3, stream_rewrite_tree_atom.nextTree())
                        self._adaptor.addChild(root_3, self._adaptor.create(EOA, "EOA"))

                        self._adaptor.addChild(root_2, root_3)
                        self._adaptor.addChild(root_2, self._adaptor.create(EOB, "EOB"))

                        self._adaptor.addChild(root_1, root_2)

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0


                elif alt67 == 3:
                    # /home/res/ANTLRv3_py.g:373:6: rewrite_tree ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] rewrite_tree EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> rewrite_tree )
                    pass 
                    self._state.following.append(self.FOLLOW_rewrite_tree_in_rewrite_tree_element2725)
                    rewrite_tree140 = self.rewrite_tree()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_rewrite_tree.add(rewrite_tree140.tree)
                    # /home/res/ANTLRv3_py.g:374:3: ( ebnfSuffix -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] rewrite_tree EOA[\"EOA\"] ) EOB[\"EOB\"] ) ) | -> rewrite_tree )
                    alt66 = 2
                    LA66_0 = self.input.LA(1)

                    if (LA66_0 == 74 or (90 <= LA66_0 <= 91)) :
                        alt66 = 1
                    elif (LA66_0 == EOF or LA66_0 == TREE_BEGIN or LA66_0 == REWRITE or (TOKEN_REF <= LA66_0 <= ACTION) or LA66_0 == RULE_REF or LA66_0 == 69 or (82 <= LA66_0 <= 84) or LA66_0 == 93) :
                        alt66 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        nvae = NoViableAltException("", 66, 0, self.input)

                        raise nvae

                    if alt66 == 1:
                        # /home/res/ANTLRv3_py.g:374:5: ebnfSuffix
                        pass 
                        self._state.following.append(self.FOLLOW_ebnfSuffix_in_rewrite_tree_element2731)
                        ebnfSuffix141 = self.ebnfSuffix()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_ebnfSuffix.add(ebnfSuffix141.tree)

                        # AST Rewrite
                        # elements: ebnfSuffix, rewrite_tree
                        # token labels: 
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 375:4: -> ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] rewrite_tree EOA[\"EOA\"] ) EOB[\"EOB\"] ) )
                            # /home/res/ANTLRv3_py.g:375:7: ^( ebnfSuffix ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] rewrite_tree EOA[\"EOA\"] ) EOB[\"EOB\"] ) )
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(stream_ebnfSuffix.nextNode(), root_1)

                            # /home/res/ANTLRv3_py.g:375:20: ^( BLOCK[\"BLOCK\"] ^( ALT[\"ALT\"] rewrite_tree EOA[\"EOA\"] ) EOB[\"EOB\"] )
                            root_2 = self._adaptor.nil()
                            root_2 = self._adaptor.becomeRoot(self._adaptor.create(BLOCK, "BLOCK"), root_2)

                            # /home/res/ANTLRv3_py.g:375:37: ^( ALT[\"ALT\"] rewrite_tree EOA[\"EOA\"] )
                            root_3 = self._adaptor.nil()
                            root_3 = self._adaptor.becomeRoot(self._adaptor.create(ALT, "ALT"), root_3)

                            self._adaptor.addChild(root_3, stream_rewrite_tree.nextTree())
                            self._adaptor.addChild(root_3, self._adaptor.create(EOA, "EOA"))

                            self._adaptor.addChild(root_2, root_3)
                            self._adaptor.addChild(root_2, self._adaptor.create(EOB, "EOB"))

                            self._adaptor.addChild(root_1, root_2)

                            self._adaptor.addChild(root_0, root_1)



                            retval.tree = root_0


                    elif alt66 == 2:
                        # /home/res/ANTLRv3_py.g:376:5: 
                        pass 
                        # AST Rewrite
                        # elements: rewrite_tree
                        # token labels: 
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:

                            retval.tree = root_0

                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 376:5: -> rewrite_tree
                            self._adaptor.addChild(root_0, stream_rewrite_tree.nextTree())



                            retval.tree = root_0





                elif alt67 == 4:
                    # /home/res/ANTLRv3_py.g:378:6: rewrite_tree_ebnf
                    pass 
                    root_0 = self._adaptor.nil()

                    self._state.following.append(self.FOLLOW_rewrite_tree_ebnf_in_rewrite_tree_element2777)
                    rewrite_tree_ebnf142 = self.rewrite_tree_ebnf()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, rewrite_tree_ebnf142.tree)


                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "rewrite_tree_element"

    class rewrite_tree_atom_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.rewrite_tree_atom_return, self).__init__()

            self.tree = None




    # $ANTLR start "rewrite_tree_atom"
    # /home/res/ANTLRv3_py.g:381:1: rewrite_tree_atom : ( CHAR_LITERAL | TOKEN_REF ( ARG_ACTION )? -> ^( TOKEN_REF ( ARG_ACTION )? ) | RULE_REF | STRING_LITERAL | d= '$' id -> LABEL[$d,$id.text] | ACTION );
    def rewrite_tree_atom(self, ):

        retval = self.rewrite_tree_atom_return()
        retval.start = self.input.LT(1)

        root_0 = None

        d = None
        CHAR_LITERAL143 = None
        TOKEN_REF144 = None
        ARG_ACTION145 = None
        RULE_REF146 = None
        STRING_LITERAL147 = None
        ACTION149 = None
        id148 = None


        d_tree = None
        CHAR_LITERAL143_tree = None
        TOKEN_REF144_tree = None
        ARG_ACTION145_tree = None
        RULE_REF146_tree = None
        STRING_LITERAL147_tree = None
        ACTION149_tree = None
        stream_93 = RewriteRuleTokenStream(self._adaptor, "token 93")
        stream_TOKEN_REF = RewriteRuleTokenStream(self._adaptor, "token TOKEN_REF")
        stream_ARG_ACTION = RewriteRuleTokenStream(self._adaptor, "token ARG_ACTION")
        stream_id = RewriteRuleSubtreeStream(self._adaptor, "rule id")
        try:
            try:
                # /home/res/ANTLRv3_py.g:382:5: ( CHAR_LITERAL | TOKEN_REF ( ARG_ACTION )? -> ^( TOKEN_REF ( ARG_ACTION )? ) | RULE_REF | STRING_LITERAL | d= '$' id -> LABEL[$d,$id.text] | ACTION )
                alt69 = 6
                LA69 = self.input.LA(1)
                if LA69 == CHAR_LITERAL:
                    alt69 = 1
                elif LA69 == TOKEN_REF:
                    alt69 = 2
                elif LA69 == RULE_REF:
                    alt69 = 3
                elif LA69 == STRING_LITERAL:
                    alt69 = 4
                elif LA69 == 93:
                    alt69 = 5
                elif LA69 == ACTION:
                    alt69 = 6
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed

                    nvae = NoViableAltException("", 69, 0, self.input)

                    raise nvae

                if alt69 == 1:
                    # /home/res/ANTLRv3_py.g:382:9: CHAR_LITERAL
                    pass 
                    root_0 = self._adaptor.nil()

                    CHAR_LITERAL143=self.match(self.input, CHAR_LITERAL, self.FOLLOW_CHAR_LITERAL_in_rewrite_tree_atom2793)
                    if self._state.backtracking == 0:

                        CHAR_LITERAL143_tree = self._adaptor.createWithPayload(CHAR_LITERAL143)
                        self._adaptor.addChild(root_0, CHAR_LITERAL143_tree)



                elif alt69 == 2:
                    # /home/res/ANTLRv3_py.g:383:6: TOKEN_REF ( ARG_ACTION )?
                    pass 
                    TOKEN_REF144=self.match(self.input, TOKEN_REF, self.FOLLOW_TOKEN_REF_in_rewrite_tree_atom2800) 
                    if self._state.backtracking == 0:
                        stream_TOKEN_REF.add(TOKEN_REF144)
                    # /home/res/ANTLRv3_py.g:383:16: ( ARG_ACTION )?
                    alt68 = 2
                    LA68_0 = self.input.LA(1)

                    if (LA68_0 == ARG_ACTION) :
                        alt68 = 1
                    if alt68 == 1:
                        # /home/res/ANTLRv3_py.g:383:16: ARG_ACTION
                        pass 
                        ARG_ACTION145=self.match(self.input, ARG_ACTION, self.FOLLOW_ARG_ACTION_in_rewrite_tree_atom2802) 
                        if self._state.backtracking == 0:
                            stream_ARG_ACTION.add(ARG_ACTION145)




                    # AST Rewrite
                    # elements: TOKEN_REF, ARG_ACTION
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 383:28: -> ^( TOKEN_REF ( ARG_ACTION )? )
                        # /home/res/ANTLRv3_py.g:383:31: ^( TOKEN_REF ( ARG_ACTION )? )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(stream_TOKEN_REF.nextNode(), root_1)

                        # /home/res/ANTLRv3_py.g:383:43: ( ARG_ACTION )?
                        if stream_ARG_ACTION.hasNext():
                            self._adaptor.addChild(root_1, stream_ARG_ACTION.nextNode())


                        stream_ARG_ACTION.reset();

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0


                elif alt69 == 3:
                    # /home/res/ANTLRv3_py.g:384:9: RULE_REF
                    pass 
                    root_0 = self._adaptor.nil()

                    RULE_REF146=self.match(self.input, RULE_REF, self.FOLLOW_RULE_REF_in_rewrite_tree_atom2823)
                    if self._state.backtracking == 0:

                        RULE_REF146_tree = self._adaptor.createWithPayload(RULE_REF146)
                        self._adaptor.addChild(root_0, RULE_REF146_tree)



                elif alt69 == 4:
                    # /home/res/ANTLRv3_py.g:385:6: STRING_LITERAL
                    pass 
                    root_0 = self._adaptor.nil()

                    STRING_LITERAL147=self.match(self.input, STRING_LITERAL, self.FOLLOW_STRING_LITERAL_in_rewrite_tree_atom2830)
                    if self._state.backtracking == 0:

                        STRING_LITERAL147_tree = self._adaptor.createWithPayload(STRING_LITERAL147)
                        self._adaptor.addChild(root_0, STRING_LITERAL147_tree)



                elif alt69 == 5:
                    # /home/res/ANTLRv3_py.g:386:6: d= '$' id
                    pass 
                    d=self.match(self.input, 93, self.FOLLOW_93_in_rewrite_tree_atom2839) 
                    if self._state.backtracking == 0:
                        stream_93.add(d)
                    self._state.following.append(self.FOLLOW_id_in_rewrite_tree_atom2841)
                    id148 = self.id()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_id.add(id148.tree)

                    # AST Rewrite
                    # elements: 
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 386:15: -> LABEL[$d,$id.text]
                        self._adaptor.addChild(root_0, self._adaptor.create(LABEL, d, ((id148 is not None) and [self.input.toString(id148.start,id148.stop)] or [None])[0]))



                        retval.tree = root_0


                elif alt69 == 6:
                    # /home/res/ANTLRv3_py.g:387:4: ACTION
                    pass 
                    root_0 = self._adaptor.nil()

                    ACTION149=self.match(self.input, ACTION, self.FOLLOW_ACTION_in_rewrite_tree_atom2852)
                    if self._state.backtracking == 0:

                        ACTION149_tree = self._adaptor.createWithPayload(ACTION149)
                        self._adaptor.addChild(root_0, ACTION149_tree)



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "rewrite_tree_atom"

    class rewrite_tree_ebnf_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.rewrite_tree_ebnf_return, self).__init__()

            self.tree = None




    # $ANTLR start "rewrite_tree_ebnf"
    # /home/res/ANTLRv3_py.g:390:1: rewrite_tree_ebnf : rewrite_tree_block ebnfSuffix -> ^( ebnfSuffix rewrite_tree_block ) ;
    def rewrite_tree_ebnf(self, ):

        retval = self.rewrite_tree_ebnf_return()
        retval.start = self.input.LT(1)

        root_0 = None

        rewrite_tree_block150 = None

        ebnfSuffix151 = None


        stream_ebnfSuffix = RewriteRuleSubtreeStream(self._adaptor, "rule ebnfSuffix")
        stream_rewrite_tree_block = RewriteRuleSubtreeStream(self._adaptor, "rule rewrite_tree_block")
               
        #    Token firstToken = input.LT(1);
        firstToken = self.input.LT(1)

        try:
            try:
                # /home/res/ANTLRv3_py.g:399:2: ( rewrite_tree_block ebnfSuffix -> ^( ebnfSuffix rewrite_tree_block ) )
                # /home/res/ANTLRv3_py.g:399:4: rewrite_tree_block ebnfSuffix
                pass 
                self._state.following.append(self.FOLLOW_rewrite_tree_block_in_rewrite_tree_ebnf2873)
                rewrite_tree_block150 = self.rewrite_tree_block()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_rewrite_tree_block.add(rewrite_tree_block150.tree)
                self._state.following.append(self.FOLLOW_ebnfSuffix_in_rewrite_tree_ebnf2875)
                ebnfSuffix151 = self.ebnfSuffix()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_ebnfSuffix.add(ebnfSuffix151.tree)

                # AST Rewrite
                # elements: rewrite_tree_block, ebnfSuffix
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 399:34: -> ^( ebnfSuffix rewrite_tree_block )
                    # /home/res/ANTLRv3_py.g:399:37: ^( ebnfSuffix rewrite_tree_block )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(stream_ebnfSuffix.nextNode(), root_1)

                    self._adaptor.addChild(root_1, stream_rewrite_tree_block.nextTree())

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)

                if self._state.backtracking == 0:
                            
                    retval.tree.getToken().setLine(firstToken.getLine());
                    retval.tree.getToken().setCharPositionInLine(firstToken.getCharPositionInLine());



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "rewrite_tree_ebnf"

    class rewrite_tree_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.rewrite_tree_return, self).__init__()

            self.tree = None




    # $ANTLR start "rewrite_tree"
    # /home/res/ANTLRv3_py.g:402:1: rewrite_tree : '^(' rewrite_tree_atom ( rewrite_tree_element )* ')' -> ^( TREE_BEGIN rewrite_tree_atom ( rewrite_tree_element )* ) ;
    def rewrite_tree(self, ):

        retval = self.rewrite_tree_return()
        retval.start = self.input.LT(1)

        root_0 = None

        string_literal152 = None
        char_literal155 = None
        rewrite_tree_atom153 = None

        rewrite_tree_element154 = None


        string_literal152_tree = None
        char_literal155_tree = None
        stream_TREE_BEGIN = RewriteRuleTokenStream(self._adaptor, "token TREE_BEGIN")
        stream_84 = RewriteRuleTokenStream(self._adaptor, "token 84")
        stream_rewrite_tree_element = RewriteRuleSubtreeStream(self._adaptor, "rule rewrite_tree_element")
        stream_rewrite_tree_atom = RewriteRuleSubtreeStream(self._adaptor, "rule rewrite_tree_atom")
        try:
            try:
                # /home/res/ANTLRv3_py.g:403:2: ( '^(' rewrite_tree_atom ( rewrite_tree_element )* ')' -> ^( TREE_BEGIN rewrite_tree_atom ( rewrite_tree_element )* ) )
                # /home/res/ANTLRv3_py.g:403:4: '^(' rewrite_tree_atom ( rewrite_tree_element )* ')'
                pass 
                string_literal152=self.match(self.input, TREE_BEGIN, self.FOLLOW_TREE_BEGIN_in_rewrite_tree2895) 
                if self._state.backtracking == 0:
                    stream_TREE_BEGIN.add(string_literal152)
                self._state.following.append(self.FOLLOW_rewrite_tree_atom_in_rewrite_tree2897)
                rewrite_tree_atom153 = self.rewrite_tree_atom()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_rewrite_tree_atom.add(rewrite_tree_atom153.tree)
                # /home/res/ANTLRv3_py.g:403:27: ( rewrite_tree_element )*
                while True: #loop70
                    alt70 = 2
                    LA70_0 = self.input.LA(1)

                    if (LA70_0 == TREE_BEGIN or (TOKEN_REF <= LA70_0 <= ACTION) or LA70_0 == RULE_REF or LA70_0 == 82 or LA70_0 == 93) :
                        alt70 = 1


                    if alt70 == 1:
                        # /home/res/ANTLRv3_py.g:403:27: rewrite_tree_element
                        pass 
                        self._state.following.append(self.FOLLOW_rewrite_tree_element_in_rewrite_tree2899)
                        rewrite_tree_element154 = self.rewrite_tree_element()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_rewrite_tree_element.add(rewrite_tree_element154.tree)


                    else:
                        break #loop70
                char_literal155=self.match(self.input, 84, self.FOLLOW_84_in_rewrite_tree2902) 
                if self._state.backtracking == 0:
                    stream_84.add(char_literal155)

                # AST Rewrite
                # elements: rewrite_tree_atom, rewrite_tree_element
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 404:3: -> ^( TREE_BEGIN rewrite_tree_atom ( rewrite_tree_element )* )
                    # /home/res/ANTLRv3_py.g:404:6: ^( TREE_BEGIN rewrite_tree_atom ( rewrite_tree_element )* )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(self._adaptor.createFromType(TREE_BEGIN, "TREE_BEGIN"), root_1)

                    self._adaptor.addChild(root_1, stream_rewrite_tree_atom.nextTree())
                    # /home/res/ANTLRv3_py.g:404:37: ( rewrite_tree_element )*
                    while stream_rewrite_tree_element.hasNext():
                        self._adaptor.addChild(root_1, stream_rewrite_tree_element.nextTree())


                    stream_rewrite_tree_element.reset();

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "rewrite_tree"

    class rewrite_template_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.rewrite_template_return, self).__init__()

            self.tree = None




    # $ANTLR start "rewrite_template"
    # /home/res/ANTLRv3_py.g:407:1: rewrite_template : ( id lp= '(' rewrite_template_args ')' (str= DOUBLE_QUOTE_STRING_LITERAL | str= DOUBLE_ANGLE_STRING_LITERAL ) -> ^( TEMPLATE[$lp,\"TEMPLATE\"] id rewrite_template_args $str) | rewrite_template_ref | rewrite_indirect_template_head | ACTION );
    def rewrite_template(self, ):

        retval = self.rewrite_template_return()
        retval.start = self.input.LT(1)

        root_0 = None

        lp = None
        str = None
        char_literal158 = None
        ACTION161 = None
        id156 = None

        rewrite_template_args157 = None

        rewrite_template_ref159 = None

        rewrite_indirect_template_head160 = None


        lp_tree = None
        str_tree = None
        char_literal158_tree = None
        ACTION161_tree = None
        stream_DOUBLE_QUOTE_STRING_LITERAL = RewriteRuleTokenStream(self._adaptor, "token DOUBLE_QUOTE_STRING_LITERAL")
        stream_82 = RewriteRuleTokenStream(self._adaptor, "token 82")
        stream_DOUBLE_ANGLE_STRING_LITERAL = RewriteRuleTokenStream(self._adaptor, "token DOUBLE_ANGLE_STRING_LITERAL")
        stream_84 = RewriteRuleTokenStream(self._adaptor, "token 84")
        stream_id = RewriteRuleSubtreeStream(self._adaptor, "rule id")
        stream_rewrite_template_args = RewriteRuleSubtreeStream(self._adaptor, "rule rewrite_template_args")
        try:
            try:
                # /home/res/ANTLRv3_py.g:419:2: ( id lp= '(' rewrite_template_args ')' (str= DOUBLE_QUOTE_STRING_LITERAL | str= DOUBLE_ANGLE_STRING_LITERAL ) -> ^( TEMPLATE[$lp,\"TEMPLATE\"] id rewrite_template_args $str) | rewrite_template_ref | rewrite_indirect_template_head | ACTION )
                alt72 = 4
                alt72 = self.dfa72.predict(self.input)
                if alt72 == 1:
                    # /home/res/ANTLRv3_py.g:420:3: id lp= '(' rewrite_template_args ')' (str= DOUBLE_QUOTE_STRING_LITERAL | str= DOUBLE_ANGLE_STRING_LITERAL )
                    pass 
                    self._state.following.append(self.FOLLOW_id_in_rewrite_template2934)
                    id156 = self.id()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_id.add(id156.tree)
                    lp=self.match(self.input, 82, self.FOLLOW_82_in_rewrite_template2938) 
                    if self._state.backtracking == 0:
                        stream_82.add(lp)
                    self._state.following.append(self.FOLLOW_rewrite_template_args_in_rewrite_template2940)
                    rewrite_template_args157 = self.rewrite_template_args()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_rewrite_template_args.add(rewrite_template_args157.tree)
                    char_literal158=self.match(self.input, 84, self.FOLLOW_84_in_rewrite_template2942) 
                    if self._state.backtracking == 0:
                        stream_84.add(char_literal158)
                    # /home/res/ANTLRv3_py.g:421:3: (str= DOUBLE_QUOTE_STRING_LITERAL | str= DOUBLE_ANGLE_STRING_LITERAL )
                    alt71 = 2
                    LA71_0 = self.input.LA(1)

                    if (LA71_0 == DOUBLE_QUOTE_STRING_LITERAL) :
                        alt71 = 1
                    elif (LA71_0 == DOUBLE_ANGLE_STRING_LITERAL) :
                        alt71 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed

                        nvae = NoViableAltException("", 71, 0, self.input)

                        raise nvae

                    if alt71 == 1:
                        # /home/res/ANTLRv3_py.g:421:5: str= DOUBLE_QUOTE_STRING_LITERAL
                        pass 
                        str=self.match(self.input, DOUBLE_QUOTE_STRING_LITERAL, self.FOLLOW_DOUBLE_QUOTE_STRING_LITERAL_in_rewrite_template2950) 
                        if self._state.backtracking == 0:
                            stream_DOUBLE_QUOTE_STRING_LITERAL.add(str)


                    elif alt71 == 2:
                        # /home/res/ANTLRv3_py.g:421:39: str= DOUBLE_ANGLE_STRING_LITERAL
                        pass 
                        str=self.match(self.input, DOUBLE_ANGLE_STRING_LITERAL, self.FOLLOW_DOUBLE_ANGLE_STRING_LITERAL_in_rewrite_template2956) 
                        if self._state.backtracking == 0:
                            stream_DOUBLE_ANGLE_STRING_LITERAL.add(str)




                    # AST Rewrite
                    # elements: str, rewrite_template_args, id
                    # token labels: str
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0
                        stream_str = RewriteRuleTokenStream(self._adaptor, "token str", str)

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 422:3: -> ^( TEMPLATE[$lp,\"TEMPLATE\"] id rewrite_template_args $str)
                        # /home/res/ANTLRv3_py.g:422:6: ^( TEMPLATE[$lp,\"TEMPLATE\"] id rewrite_template_args $str)
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(self._adaptor.create(TEMPLATE, lp, "TEMPLATE"), root_1)

                        self._adaptor.addChild(root_1, stream_id.nextTree())
                        self._adaptor.addChild(root_1, stream_rewrite_template_args.nextTree())
                        self._adaptor.addChild(root_1, stream_str.nextNode())

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0


                elif alt72 == 2:
                    # /home/res/ANTLRv3_py.g:425:3: rewrite_template_ref
                    pass 
                    root_0 = self._adaptor.nil()

                    self._state.following.append(self.FOLLOW_rewrite_template_ref_in_rewrite_template2983)
                    rewrite_template_ref159 = self.rewrite_template_ref()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, rewrite_template_ref159.tree)


                elif alt72 == 3:
                    # /home/res/ANTLRv3_py.g:428:3: rewrite_indirect_template_head
                    pass 
                    root_0 = self._adaptor.nil()

                    self._state.following.append(self.FOLLOW_rewrite_indirect_template_head_in_rewrite_template2992)
                    rewrite_indirect_template_head160 = self.rewrite_indirect_template_head()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, rewrite_indirect_template_head160.tree)


                elif alt72 == 4:
                    # /home/res/ANTLRv3_py.g:431:3: ACTION
                    pass 
                    root_0 = self._adaptor.nil()

                    ACTION161=self.match(self.input, ACTION, self.FOLLOW_ACTION_in_rewrite_template3001)
                    if self._state.backtracking == 0:

                        ACTION161_tree = self._adaptor.createWithPayload(ACTION161)
                        self._adaptor.addChild(root_0, ACTION161_tree)



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "rewrite_template"

    class rewrite_template_ref_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.rewrite_template_ref_return, self).__init__()

            self.tree = None




    # $ANTLR start "rewrite_template_ref"
    # /home/res/ANTLRv3_py.g:434:1: rewrite_template_ref : id lp= '(' rewrite_template_args ')' -> ^( TEMPLATE[$lp,\"TEMPLATE\"] id rewrite_template_args ) ;
    def rewrite_template_ref(self, ):

        retval = self.rewrite_template_ref_return()
        retval.start = self.input.LT(1)

        root_0 = None

        lp = None
        char_literal164 = None
        id162 = None

        rewrite_template_args163 = None


        lp_tree = None
        char_literal164_tree = None
        stream_82 = RewriteRuleTokenStream(self._adaptor, "token 82")
        stream_84 = RewriteRuleTokenStream(self._adaptor, "token 84")
        stream_id = RewriteRuleSubtreeStream(self._adaptor, "rule id")
        stream_rewrite_template_args = RewriteRuleSubtreeStream(self._adaptor, "rule rewrite_template_args")
        try:
            try:
                # /home/res/ANTLRv3_py.g:436:2: ( id lp= '(' rewrite_template_args ')' -> ^( TEMPLATE[$lp,\"TEMPLATE\"] id rewrite_template_args ) )
                # /home/res/ANTLRv3_py.g:436:4: id lp= '(' rewrite_template_args ')'
                pass 
                self._state.following.append(self.FOLLOW_id_in_rewrite_template_ref3014)
                id162 = self.id()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_id.add(id162.tree)
                lp=self.match(self.input, 82, self.FOLLOW_82_in_rewrite_template_ref3018) 
                if self._state.backtracking == 0:
                    stream_82.add(lp)
                self._state.following.append(self.FOLLOW_rewrite_template_args_in_rewrite_template_ref3020)
                rewrite_template_args163 = self.rewrite_template_args()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_rewrite_template_args.add(rewrite_template_args163.tree)
                char_literal164=self.match(self.input, 84, self.FOLLOW_84_in_rewrite_template_ref3022) 
                if self._state.backtracking == 0:
                    stream_84.add(char_literal164)

                # AST Rewrite
                # elements: id, rewrite_template_args
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 437:3: -> ^( TEMPLATE[$lp,\"TEMPLATE\"] id rewrite_template_args )
                    # /home/res/ANTLRv3_py.g:437:6: ^( TEMPLATE[$lp,\"TEMPLATE\"] id rewrite_template_args )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(self._adaptor.create(TEMPLATE, lp, "TEMPLATE"), root_1)

                    self._adaptor.addChild(root_1, stream_id.nextTree())
                    self._adaptor.addChild(root_1, stream_rewrite_template_args.nextTree())

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "rewrite_template_ref"

    class rewrite_indirect_template_head_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.rewrite_indirect_template_head_return, self).__init__()

            self.tree = None




    # $ANTLR start "rewrite_indirect_template_head"
    # /home/res/ANTLRv3_py.g:440:1: rewrite_indirect_template_head : lp= '(' ACTION ')' '(' rewrite_template_args ')' -> ^( TEMPLATE[$lp,\"TEMPLATE\"] ACTION rewrite_template_args ) ;
    def rewrite_indirect_template_head(self, ):

        retval = self.rewrite_indirect_template_head_return()
        retval.start = self.input.LT(1)

        root_0 = None

        lp = None
        ACTION165 = None
        char_literal166 = None
        char_literal167 = None
        char_literal169 = None
        rewrite_template_args168 = None


        lp_tree = None
        ACTION165_tree = None
        char_literal166_tree = None
        char_literal167_tree = None
        char_literal169_tree = None
        stream_82 = RewriteRuleTokenStream(self._adaptor, "token 82")
        stream_ACTION = RewriteRuleTokenStream(self._adaptor, "token ACTION")
        stream_84 = RewriteRuleTokenStream(self._adaptor, "token 84")
        stream_rewrite_template_args = RewriteRuleSubtreeStream(self._adaptor, "rule rewrite_template_args")
        try:
            try:
                # /home/res/ANTLRv3_py.g:442:2: (lp= '(' ACTION ')' '(' rewrite_template_args ')' -> ^( TEMPLATE[$lp,\"TEMPLATE\"] ACTION rewrite_template_args ) )
                # /home/res/ANTLRv3_py.g:442:4: lp= '(' ACTION ')' '(' rewrite_template_args ')'
                pass 
                lp=self.match(self.input, 82, self.FOLLOW_82_in_rewrite_indirect_template_head3050) 
                if self._state.backtracking == 0:
                    stream_82.add(lp)
                ACTION165=self.match(self.input, ACTION, self.FOLLOW_ACTION_in_rewrite_indirect_template_head3052) 
                if self._state.backtracking == 0:
                    stream_ACTION.add(ACTION165)
                char_literal166=self.match(self.input, 84, self.FOLLOW_84_in_rewrite_indirect_template_head3054) 
                if self._state.backtracking == 0:
                    stream_84.add(char_literal166)
                char_literal167=self.match(self.input, 82, self.FOLLOW_82_in_rewrite_indirect_template_head3056) 
                if self._state.backtracking == 0:
                    stream_82.add(char_literal167)
                self._state.following.append(self.FOLLOW_rewrite_template_args_in_rewrite_indirect_template_head3058)
                rewrite_template_args168 = self.rewrite_template_args()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_rewrite_template_args.add(rewrite_template_args168.tree)
                char_literal169=self.match(self.input, 84, self.FOLLOW_84_in_rewrite_indirect_template_head3060) 
                if self._state.backtracking == 0:
                    stream_84.add(char_literal169)

                # AST Rewrite
                # elements: ACTION, rewrite_template_args
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 443:3: -> ^( TEMPLATE[$lp,\"TEMPLATE\"] ACTION rewrite_template_args )
                    # /home/res/ANTLRv3_py.g:443:6: ^( TEMPLATE[$lp,\"TEMPLATE\"] ACTION rewrite_template_args )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(self._adaptor.create(TEMPLATE, lp, "TEMPLATE"), root_1)

                    self._adaptor.addChild(root_1, stream_ACTION.nextNode())
                    self._adaptor.addChild(root_1, stream_rewrite_template_args.nextTree())

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "rewrite_indirect_template_head"

    class rewrite_template_args_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.rewrite_template_args_return, self).__init__()

            self.tree = None




    # $ANTLR start "rewrite_template_args"
    # /home/res/ANTLRv3_py.g:446:1: rewrite_template_args : ( rewrite_template_arg ( ',' rewrite_template_arg )* -> ^( ARGLIST ( rewrite_template_arg )+ ) | -> ARGLIST );
    def rewrite_template_args(self, ):

        retval = self.rewrite_template_args_return()
        retval.start = self.input.LT(1)

        root_0 = None

        char_literal171 = None
        rewrite_template_arg170 = None

        rewrite_template_arg172 = None


        char_literal171_tree = None
        stream_81 = RewriteRuleTokenStream(self._adaptor, "token 81")
        stream_rewrite_template_arg = RewriteRuleSubtreeStream(self._adaptor, "rule rewrite_template_arg")
        try:
            try:
                # /home/res/ANTLRv3_py.g:447:2: ( rewrite_template_arg ( ',' rewrite_template_arg )* -> ^( ARGLIST ( rewrite_template_arg )+ ) | -> ARGLIST )
                alt74 = 2
                LA74_0 = self.input.LA(1)

                if (LA74_0 == TOKEN_REF or LA74_0 == RULE_REF) :
                    alt74 = 1
                elif (LA74_0 == 84) :
                    alt74 = 2
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed

                    nvae = NoViableAltException("", 74, 0, self.input)

                    raise nvae

                if alt74 == 1:
                    # /home/res/ANTLRv3_py.g:447:4: rewrite_template_arg ( ',' rewrite_template_arg )*
                    pass 
                    self._state.following.append(self.FOLLOW_rewrite_template_arg_in_rewrite_template_args3084)
                    rewrite_template_arg170 = self.rewrite_template_arg()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_rewrite_template_arg.add(rewrite_template_arg170.tree)
                    # /home/res/ANTLRv3_py.g:447:25: ( ',' rewrite_template_arg )*
                    while True: #loop73
                        alt73 = 2
                        LA73_0 = self.input.LA(1)

                        if (LA73_0 == 81) :
                            alt73 = 1


                        if alt73 == 1:
                            # /home/res/ANTLRv3_py.g:447:26: ',' rewrite_template_arg
                            pass 
                            char_literal171=self.match(self.input, 81, self.FOLLOW_81_in_rewrite_template_args3087) 
                            if self._state.backtracking == 0:
                                stream_81.add(char_literal171)
                            self._state.following.append(self.FOLLOW_rewrite_template_arg_in_rewrite_template_args3089)
                            rewrite_template_arg172 = self.rewrite_template_arg()

                            self._state.following.pop()
                            if self._state.backtracking == 0:
                                stream_rewrite_template_arg.add(rewrite_template_arg172.tree)


                        else:
                            break #loop73

                    # AST Rewrite
                    # elements: rewrite_template_arg
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 448:3: -> ^( ARGLIST ( rewrite_template_arg )+ )
                        # /home/res/ANTLRv3_py.g:448:6: ^( ARGLIST ( rewrite_template_arg )+ )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(self._adaptor.createFromType(ARGLIST, "ARGLIST"), root_1)

                        # /home/res/ANTLRv3_py.g:448:16: ( rewrite_template_arg )+
                        if not (stream_rewrite_template_arg.hasNext()):
                            raise RewriteEarlyExitException()

                        while stream_rewrite_template_arg.hasNext():
                            self._adaptor.addChild(root_1, stream_rewrite_template_arg.nextTree())


                        stream_rewrite_template_arg.reset()

                        self._adaptor.addChild(root_0, root_1)



                        retval.tree = root_0


                elif alt74 == 2:
                    # /home/res/ANTLRv3_py.g:449:4: 
                    pass 
                    # AST Rewrite
                    # elements: 
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 449:4: -> ARGLIST
                        self._adaptor.addChild(root_0, self._adaptor.createFromType(ARGLIST, "ARGLIST"))



                        retval.tree = root_0


                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "rewrite_template_args"

    class rewrite_template_arg_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.rewrite_template_arg_return, self).__init__()

            self.tree = None




    # $ANTLR start "rewrite_template_arg"
    # /home/res/ANTLRv3_py.g:452:1: rewrite_template_arg : id '=' ACTION -> ^( ARG[$id.start] id ACTION ) ;
    def rewrite_template_arg(self, ):

        retval = self.rewrite_template_arg_return()
        retval.start = self.input.LT(1)

        root_0 = None

        char_literal174 = None
        ACTION175 = None
        id173 = None


        char_literal174_tree = None
        ACTION175_tree = None
        stream_71 = RewriteRuleTokenStream(self._adaptor, "token 71")
        stream_ACTION = RewriteRuleTokenStream(self._adaptor, "token ACTION")
        stream_id = RewriteRuleSubtreeStream(self._adaptor, "rule id")
        try:
            try:
                # /home/res/ANTLRv3_py.g:453:2: ( id '=' ACTION -> ^( ARG[$id.start] id ACTION ) )
                # /home/res/ANTLRv3_py.g:453:6: id '=' ACTION
                pass 
                self._state.following.append(self.FOLLOW_id_in_rewrite_template_arg3122)
                id173 = self.id()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_id.add(id173.tree)
                char_literal174=self.match(self.input, 71, self.FOLLOW_71_in_rewrite_template_arg3124) 
                if self._state.backtracking == 0:
                    stream_71.add(char_literal174)
                ACTION175=self.match(self.input, ACTION, self.FOLLOW_ACTION_in_rewrite_template_arg3126) 
                if self._state.backtracking == 0:
                    stream_ACTION.add(ACTION175)

                # AST Rewrite
                # elements: ACTION, id
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:

                    retval.tree = root_0

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 453:20: -> ^( ARG[$id.start] id ACTION )
                    # /home/res/ANTLRv3_py.g:453:23: ^( ARG[$id.start] id ACTION )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(self._adaptor.create(ARG, id173.start), root_1)

                    self._adaptor.addChild(root_1, stream_id.nextTree())
                    self._adaptor.addChild(root_1, stream_ACTION.nextNode())

                    self._adaptor.addChild(root_0, root_1)



                    retval.tree = root_0



                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "rewrite_template_arg"

    class id_return(ParserRuleReturnScope):
        def __init__(self):
            super(ANTLRv3_pyParser.id_return, self).__init__()

            self.tree = None




    # $ANTLR start "id"
    # /home/res/ANTLRv3_py.g:456:1: id : ( TOKEN_REF -> ID[$TOKEN_REF] | RULE_REF -> ID[$RULE_REF] );
    def id(self, ):

        retval = self.id_return()
        retval.start = self.input.LT(1)

        root_0 = None

        TOKEN_REF176 = None
        RULE_REF177 = None

        TOKEN_REF176_tree = None
        RULE_REF177_tree = None
        stream_RULE_REF = RewriteRuleTokenStream(self._adaptor, "token RULE_REF")
        stream_TOKEN_REF = RewriteRuleTokenStream(self._adaptor, "token TOKEN_REF")

        try:
            try:
                # /home/res/ANTLRv3_py.g:456:4: ( TOKEN_REF -> ID[$TOKEN_REF] | RULE_REF -> ID[$RULE_REF] )
                alt75 = 2
                LA75_0 = self.input.LA(1)

                if (LA75_0 == TOKEN_REF) :
                    alt75 = 1
                elif (LA75_0 == RULE_REF) :
                    alt75 = 2
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed

                    nvae = NoViableAltException("", 75, 0, self.input)

                    raise nvae

                if alt75 == 1:
                    # /home/res/ANTLRv3_py.g:456:6: TOKEN_REF
                    pass 
                    TOKEN_REF176=self.match(self.input, TOKEN_REF, self.FOLLOW_TOKEN_REF_in_id3147) 
                    if self._state.backtracking == 0:
                        stream_TOKEN_REF.add(TOKEN_REF176)

                    # AST Rewrite
                    # elements: 
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 456:16: -> ID[$TOKEN_REF]
                        self._adaptor.addChild(root_0, self._adaptor.create(ID, TOKEN_REF176))



                        retval.tree = root_0


                elif alt75 == 2:
                    # /home/res/ANTLRv3_py.g:457:4: RULE_REF
                    pass 
                    RULE_REF177=self.match(self.input, RULE_REF, self.FOLLOW_RULE_REF_in_id3157) 
                    if self._state.backtracking == 0:
                        stream_RULE_REF.add(RULE_REF177)

                    # AST Rewrite
                    # elements: 
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:

                        retval.tree = root_0

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 457:14: -> ID[$RULE_REF]
                        self._adaptor.addChild(root_0, self._adaptor.create(ID, RULE_REF177))



                        retval.tree = root_0


                retval.stop = self.input.LT(-1)

                if self._state.backtracking == 0:

                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)
        finally:

            pass
        return retval

    # $ANTLR end "id"

    # $ANTLR start "synpred1_ANTLRv3_py"
    def synpred1_ANTLRv3_py_fragment(self, ):
        # /home/res/ANTLRv3_py.g:355:4: ( rewrite_template )
        # /home/res/ANTLRv3_py.g:355:4: rewrite_template
        pass 
        self._state.following.append(self.FOLLOW_rewrite_template_in_synpred1_ANTLRv3_py2571)
        self.rewrite_template()

        self._state.following.pop()


    # $ANTLR end "synpred1_ANTLRv3_py"



    # $ANTLR start "synpred2_ANTLRv3_py"
    def synpred2_ANTLRv3_py_fragment(self, ):
        # /home/res/ANTLRv3_py.g:356:4: ( rewrite_tree_alternative )
        # /home/res/ANTLRv3_py.g:356:4: rewrite_tree_alternative
        pass 
        self._state.following.append(self.FOLLOW_rewrite_tree_alternative_in_synpred2_ANTLRv3_py2576)
        self.rewrite_tree_alternative()

        self._state.following.pop()


    # $ANTLR end "synpred2_ANTLRv3_py"




    # Delegated rules

    def synpred1_ANTLRv3_py(self):
        self._state.backtracking += 1
        start = self.input.mark()
        try:
            self.synpred1_ANTLRv3_py_fragment()
        except BacktrackingFailed:
            success = False
        else:
            success = True
        self.input.rewind(start)
        self._state.backtracking -= 1
        return success

    def synpred2_ANTLRv3_py(self):
        self._state.backtracking += 1
        start = self.input.mark()
        try:
            self.synpred2_ANTLRv3_py_fragment()
        except BacktrackingFailed:
            success = False
        else:
            success = True
        self.input.rewind(start)
        self._state.backtracking -= 1
        return success



    # lookup tables for DFA #46

    DFA46_eot = DFA.unpack(
        u"\14\uffff"
        )

    DFA46_eof = DFA.unpack(
        u"\14\uffff"
        )

    DFA46_min = DFA.unpack(
        u"\3\40\5\uffff\2\52\2\uffff"
        )

    DFA46_max = DFA.unpack(
        u"\3\134\5\uffff\2\134\2\uffff"
        )

    DFA46_accept = DFA.unpack(
        u"\3\uffff\1\3\1\4\1\5\1\6\1\7\2\uffff\1\1\1\2"
        )

    DFA46_special = DFA.unpack(
        u"\14\uffff"
        )

            
    DFA46_transition = [
        DFA.unpack(u"\1\6\4\uffff\1\7\4\uffff\1\1\2\3\1\5\3\uffff\1\2\40"
        u"\uffff\1\4\6\uffff\1\3\2\uffff\1\3"),
        DFA.unpack(u"\1\3\4\uffff\4\3\1\uffff\4\3\2\uffff\2\3\23\uffff\1"
        u"\3\1\uffff\1\10\2\uffff\1\3\7\uffff\3\3\2\uffff\1\11\1\uffff\4"
        u"\3"),
        DFA.unpack(u"\1\3\4\uffff\4\3\1\uffff\4\3\2\uffff\2\3\23\uffff\1"
        u"\3\1\uffff\1\10\2\uffff\1\3\7\uffff\3\3\2\uffff\1\11\1\uffff\4"
        u"\3"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\3\12\4\uffff\1\12\40\uffff\1\13\6\uffff\1\12\2\uffff"
        u"\1\12"),
        DFA.unpack(u"\3\12\4\uffff\1\12\40\uffff\1\13\6\uffff\1\12\2\uffff"
        u"\1\12"),
        DFA.unpack(u""),
        DFA.unpack(u"")
    ]

    # class definition for DFA #46

    class DFA46(DFA):
        pass


    # lookup tables for DFA #64

    DFA64_eot = DFA.unpack(
        u"\15\uffff"
        )

    DFA64_eof = DFA.unpack(
        u"\15\uffff"
        )

    DFA64_min = DFA.unpack(
        u"\4\45\1\0\2\uffff\2\45\1\uffff\2\45\1\112"
        )

    DFA64_max = DFA.unpack(
        u"\4\135\1\0\2\uffff\2\135\1\uffff\2\135\1\133"
        )

    DFA64_accept = DFA.unpack(
        u"\5\uffff\1\2\1\3\2\uffff\1\1\3\uffff"
        )

    DFA64_special = DFA.unpack(
        u"\4\uffff\1\0\10\uffff"
        )

            
    DFA64_transition = [
        DFA.unpack(u"\1\5\2\uffff\1\6\1\uffff\1\1\2\5\1\4\3\uffff\1\2\23"
        u"\uffff\1\6\14\uffff\1\3\2\6\10\uffff\1\5"),
        DFA.unpack(u"\1\5\2\uffff\1\5\1\uffff\4\5\2\uffff\2\5\23\uffff\1"
        u"\5\4\uffff\1\5\7\uffff\1\7\2\5\5\uffff\2\5\1\uffff\1\5"),
        DFA.unpack(u"\1\5\2\uffff\1\5\1\uffff\4\5\3\uffff\1\5\23\uffff\1"
        u"\5\4\uffff\1\5\7\uffff\1\7\2\5\5\uffff\2\5\1\uffff\1\5"),
        DFA.unpack(u"\1\5\4\uffff\3\5\1\10\3\uffff\1\5\40\uffff\1\5\12\uffff"
        u"\1\5"),
        DFA.unpack(u"\1\uffff"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\5\4\uffff\1\12\3\5\3\uffff\1\13\40\uffff\1\5\1\uffff"
        u"\1\11\10\uffff\1\5"),
        DFA.unpack(u"\1\5\4\uffff\4\5\3\uffff\1\5\30\uffff\1\5\7\uffff\1"
        u"\5\1\uffff\1\14\5\uffff\2\5\1\uffff\1\5"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\5\4\uffff\4\5\2\uffff\2\5\25\uffff\1\11\2\uffff"
        u"\1\5\7\uffff\1\5\1\uffff\1\5\5\uffff\2\5\1\uffff\1\5"),
        DFA.unpack(u"\1\5\4\uffff\4\5\3\uffff\1\5\25\uffff\1\11\2\uffff"
        u"\1\5\7\uffff\1\5\1\uffff\1\5\5\uffff\2\5\1\uffff\1\5"),
        DFA.unpack(u"\1\5\7\uffff\1\11\7\uffff\2\5")
    ]

    # class definition for DFA #64

    class DFA64(DFA):
        pass


        def specialStateTransition(self_, s, input):
            # convince pylint that my self_ magic is ok ;)
            # pylint: disable-msg=E0213

            # pretend we are a member of the recognizer
            # thus semantic predicates can be evaluated
            self = self_.recognizer

            _s = s

            if s == 0: 
                LA64_4 = input.LA(1)

                 
                index64_4 = input.index()
                input.rewind()
                s = -1
                if (self.synpred1_ANTLRv3_py()):
                    s = 9

                elif (self.synpred2_ANTLRv3_py()):
                    s = 5

                 
                input.seek(index64_4)
                if s >= 0:
                    return s

            if self._state.backtracking >0:
                raise BacktrackingFailed
            nvae = NoViableAltException(self_.getDescription(), 64, _s, input)
            self_.error(nvae)
            raise nvae
    # lookup tables for DFA #67

    DFA67_eot = DFA.unpack(
        u"\16\uffff"
        )

    DFA67_eof = DFA.unpack(
        u"\1\uffff\4\11\1\uffff\1\11\4\uffff\3\11"
        )

    DFA67_min = DFA.unpack(
        u"\5\45\1\52\1\45\4\uffff\3\45"
        )

    DFA67_max = DFA.unpack(
        u"\5\135\1\61\1\135\4\uffff\3\135"
        )

    DFA67_accept = DFA.unpack(
        u"\7\uffff\1\3\1\4\1\1\1\2\3\uffff"
        )

    DFA67_special = DFA.unpack(
        u"\16\uffff"
        )

            
    DFA67_transition = [
        DFA.unpack(u"\1\7\4\uffff\1\2\1\4\1\1\1\6\3\uffff\1\3\40\uffff\1"
        u"\10\12\uffff\1\5"),
        DFA.unpack(u"\1\11\2\uffff\1\11\1\uffff\4\11\3\uffff\1\11\23\uffff"
        u"\1\11\4\uffff\1\12\7\uffff\3\11\5\uffff\2\12\1\uffff\1\11"),
        DFA.unpack(u"\1\11\2\uffff\1\11\1\uffff\4\11\2\uffff\1\13\1\11\23"
        u"\uffff\1\11\4\uffff\1\12\7\uffff\3\11\5\uffff\2\12\1\uffff\1\11"),
        DFA.unpack(u"\1\11\2\uffff\1\11\1\uffff\4\11\3\uffff\1\11\23\uffff"
        u"\1\11\4\uffff\1\12\7\uffff\3\11\5\uffff\2\12\1\uffff\1\11"),
        DFA.unpack(u"\1\11\2\uffff\1\11\1\uffff\4\11\3\uffff\1\11\23\uffff"
        u"\1\11\4\uffff\1\12\7\uffff\3\11\5\uffff\2\12\1\uffff\1\11"),
        DFA.unpack(u"\1\14\6\uffff\1\15"),
        DFA.unpack(u"\1\11\2\uffff\1\11\1\uffff\4\11\3\uffff\1\11\23\uffff"
        u"\1\11\4\uffff\1\12\7\uffff\3\11\5\uffff\2\12\1\uffff\1\11"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\11\2\uffff\1\11\1\uffff\4\11\3\uffff\1\11\23\uffff"
        u"\1\11\4\uffff\1\12\7\uffff\3\11\5\uffff\2\12\1\uffff\1\11"),
        DFA.unpack(u"\1\11\2\uffff\1\11\1\uffff\4\11\3\uffff\1\11\23\uffff"
        u"\1\11\4\uffff\1\12\7\uffff\3\11\5\uffff\2\12\1\uffff\1\11"),
        DFA.unpack(u"\1\11\2\uffff\1\11\1\uffff\4\11\3\uffff\1\11\23\uffff"
        u"\1\11\4\uffff\1\12\7\uffff\3\11\5\uffff\2\12\1\uffff\1\11")
    ]

    # class definition for DFA #67

    class DFA67(DFA):
        pass


    # lookup tables for DFA #72

    DFA72_eot = DFA.unpack(
        u"\22\uffff"
        )

    DFA72_eof = DFA.unpack(
        u"\10\uffff\1\12\11\uffff"
        )

    DFA72_min = DFA.unpack(
        u"\1\52\2\122\2\uffff\1\52\2\107\1\50\1\55\2\uffff\1\121\1\52\2\107"
        u"\1\55\1\121"
        )

    DFA72_max = DFA.unpack(
        u"\3\122\2\uffff\1\124\2\107\1\124\1\55\2\uffff\1\124\1\61\2\107"
        u"\1\55\1\124"
        )

    DFA72_accept = DFA.unpack(
        u"\3\uffff\1\3\1\4\5\uffff\1\2\1\1\6\uffff"
        )

    DFA72_special = DFA.unpack(
        u"\22\uffff"
        )

            
    DFA72_transition = [
        DFA.unpack(u"\1\1\2\uffff\1\4\3\uffff\1\2\40\uffff\1\3"),
        DFA.unpack(u"\1\5"),
        DFA.unpack(u"\1\5"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\6\6\uffff\1\7\42\uffff\1\10"),
        DFA.unpack(u"\1\11"),
        DFA.unpack(u"\1\11"),
        DFA.unpack(u"\1\12\11\uffff\2\13\21\uffff\1\12\15\uffff\2\12"),
        DFA.unpack(u"\1\14"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\15\2\uffff\1\10"),
        DFA.unpack(u"\1\16\6\uffff\1\17"),
        DFA.unpack(u"\1\20"),
        DFA.unpack(u"\1\20"),
        DFA.unpack(u"\1\21"),
        DFA.unpack(u"\1\15\2\uffff\1\10")
    ]

    # class definition for DFA #72

    class DFA72(DFA):
        pass


 

    FOLLOW_DOC_COMMENT_in_grammarDef341 = frozenset([65, 66, 67, 68])
    FOLLOW_65_in_grammarDef351 = frozenset([68])
    FOLLOW_66_in_grammarDef369 = frozenset([68])
    FOLLOW_67_in_grammarDef385 = frozenset([68])
    FOLLOW_68_in_grammarDef426 = frozenset([42, 49])
    FOLLOW_id_in_grammarDef428 = frozenset([69])
    FOLLOW_69_in_grammarDef430 = frozenset([4, 31, 36, 41, 42, 46, 49, 72, 75, 76, 77])
    FOLLOW_optionsSpec_in_grammarDef432 = frozenset([4, 31, 36, 41, 42, 46, 49, 72, 75, 76, 77])
    FOLLOW_tokensSpec_in_grammarDef435 = frozenset([4, 31, 36, 41, 42, 46, 49, 72, 75, 76, 77])
    FOLLOW_attrScope_in_grammarDef438 = frozenset([4, 31, 36, 41, 42, 46, 49, 72, 75, 76, 77])
    FOLLOW_action_in_grammarDef441 = frozenset([4, 31, 36, 41, 42, 46, 49, 72, 75, 76, 77])
    FOLLOW_rule_in_grammarDef449 = frozenset([4, 31, 36, 41, 42, 46, 49, 72, 75, 76, 77])
    FOLLOW_EOF_in_grammarDef457 = frozenset([1])
    FOLLOW_TOKENS_in_tokensSpec518 = frozenset([42])
    FOLLOW_tokenSpec_in_tokensSpec520 = frozenset([42, 70])
    FOLLOW_70_in_tokensSpec523 = frozenset([1])
    FOLLOW_TOKEN_REF_in_tokenSpec543 = frozenset([69, 71])
    FOLLOW_71_in_tokenSpec549 = frozenset([43, 44])
    FOLLOW_STRING_LITERAL_in_tokenSpec554 = frozenset([69])
    FOLLOW_CHAR_LITERAL_in_tokenSpec558 = frozenset([69])
    FOLLOW_69_in_tokenSpec597 = frozenset([1])
    FOLLOW_SCOPE_in_attrScope608 = frozenset([42, 49])
    FOLLOW_id_in_attrScope610 = frozenset([45])
    FOLLOW_ACTION_in_attrScope612 = frozenset([1])
    FOLLOW_72_in_action635 = frozenset([42, 49, 65, 66])
    FOLLOW_actionScopeName_in_action638 = frozenset([73])
    FOLLOW_73_in_action640 = frozenset([42, 49])
    FOLLOW_id_in_action644 = frozenset([45])
    FOLLOW_ACTION_in_action646 = frozenset([1])
    FOLLOW_id_in_actionScopeName672 = frozenset([1])
    FOLLOW_65_in_actionScopeName679 = frozenset([1])
    FOLLOW_66_in_actionScopeName696 = frozenset([1])
    FOLLOW_OPTIONS_in_optionsSpec712 = frozenset([42, 49])
    FOLLOW_option_in_optionsSpec715 = frozenset([69])
    FOLLOW_69_in_optionsSpec717 = frozenset([42, 49, 70])
    FOLLOW_70_in_optionsSpec721 = frozenset([1])
    FOLLOW_id_in_option746 = frozenset([71])
    FOLLOW_71_in_option748 = frozenset([42, 43, 44, 47, 49, 74])
    FOLLOW_optionValue_in_option750 = frozenset([1])
    FOLLOW_id_in_optionValue779 = frozenset([1])
    FOLLOW_STRING_LITERAL_in_optionValue789 = frozenset([1])
    FOLLOW_CHAR_LITERAL_in_optionValue799 = frozenset([1])
    FOLLOW_INT_in_optionValue809 = frozenset([1])
    FOLLOW_74_in_optionValue819 = frozenset([1])
    FOLLOW_DOC_COMMENT_in_rule844 = frozenset([4, 36, 42, 49, 75, 76, 77])
    FOLLOW_75_in_rule854 = frozenset([42, 49])
    FOLLOW_76_in_rule856 = frozenset([42, 49])
    FOLLOW_77_in_rule858 = frozenset([42, 49])
    FOLLOW_FRAGMENT_in_rule860 = frozenset([42, 49])
    FOLLOW_id_in_rule868 = frozenset([31, 39, 46, 48, 72, 78, 79, 80])
    FOLLOW_BANG_in_rule874 = frozenset([31, 46, 48, 72, 78, 79, 80])
    FOLLOW_ARG_ACTION_in_rule883 = frozenset([31, 46, 72, 78, 79, 80])
    FOLLOW_78_in_rule892 = frozenset([48])
    FOLLOW_ARG_ACTION_in_rule896 = frozenset([31, 46, 72, 79, 80])
    FOLLOW_throwsSpec_in_rule904 = frozenset([31, 46, 72, 79])
    FOLLOW_optionsSpec_in_rule907 = frozenset([31, 72, 79])
    FOLLOW_ruleScopeSpec_in_rule910 = frozenset([72, 79])
    FOLLOW_ruleAction_in_rule913 = frozenset([72, 79])
    FOLLOW_79_in_rule918 = frozenset([32, 37, 40, 42, 43, 44, 45, 49, 82, 83, 89, 92])
    FOLLOW_altList_in_rule920 = frozenset([69])
    FOLLOW_69_in_rule922 = frozenset([1, 85, 86])
    FOLLOW_exceptionGroup_in_rule926 = frozenset([1])
    FOLLOW_72_in_ruleAction1039 = frozenset([42, 49])
    FOLLOW_id_in_ruleAction1041 = frozenset([45])
    FOLLOW_ACTION_in_ruleAction1043 = frozenset([1])
    FOLLOW_80_in_throwsSpec1064 = frozenset([42, 49])
    FOLLOW_id_in_throwsSpec1066 = frozenset([1, 81])
    FOLLOW_81_in_throwsSpec1070 = frozenset([42, 49])
    FOLLOW_id_in_throwsSpec1072 = frozenset([1, 81])
    FOLLOW_SCOPE_in_ruleScopeSpec1095 = frozenset([45])
    FOLLOW_ACTION_in_ruleScopeSpec1097 = frozenset([1])
    FOLLOW_SCOPE_in_ruleScopeSpec1110 = frozenset([42, 49])
    FOLLOW_id_in_ruleScopeSpec1112 = frozenset([69, 81])
    FOLLOW_81_in_ruleScopeSpec1115 = frozenset([42, 49])
    FOLLOW_id_in_ruleScopeSpec1117 = frozenset([69, 81])
    FOLLOW_69_in_ruleScopeSpec1121 = frozenset([1])
    FOLLOW_SCOPE_in_ruleScopeSpec1135 = frozenset([45])
    FOLLOW_ACTION_in_ruleScopeSpec1137 = frozenset([31])
    FOLLOW_SCOPE_in_ruleScopeSpec1141 = frozenset([42, 49])
    FOLLOW_id_in_ruleScopeSpec1143 = frozenset([69, 81])
    FOLLOW_81_in_ruleScopeSpec1146 = frozenset([42, 49])
    FOLLOW_id_in_ruleScopeSpec1148 = frozenset([69, 81])
    FOLLOW_69_in_ruleScopeSpec1152 = frozenset([1])
    FOLLOW_82_in_block1184 = frozenset([32, 37, 40, 42, 43, 44, 45, 46, 49, 79, 82, 83, 84, 89, 92])
    FOLLOW_optionsSpec_in_block1193 = frozenset([79])
    FOLLOW_79_in_block1197 = frozenset([32, 37, 40, 42, 43, 44, 45, 49, 82, 83, 84, 89, 92])
    FOLLOW_alternative_in_block1206 = frozenset([40, 83, 84])
    FOLLOW_rewrite_in_block1208 = frozenset([83, 84])
    FOLLOW_83_in_block1212 = frozenset([32, 37, 40, 42, 43, 44, 45, 49, 82, 83, 84, 89, 92])
    FOLLOW_alternative_in_block1216 = frozenset([40, 83, 84])
    FOLLOW_rewrite_in_block1218 = frozenset([83, 84])
    FOLLOW_84_in_block1233 = frozenset([1])
    FOLLOW_alternative_in_altList1290 = frozenset([40, 83])
    FOLLOW_rewrite_in_altList1292 = frozenset([1, 83])
    FOLLOW_83_in_altList1296 = frozenset([32, 37, 40, 42, 43, 44, 45, 49, 82, 83, 89, 92])
    FOLLOW_alternative_in_altList1300 = frozenset([40, 83])
    FOLLOW_rewrite_in_altList1302 = frozenset([1, 83])
    FOLLOW_element_in_alternative1350 = frozenset([1, 32, 37, 42, 43, 44, 45, 49, 82, 89, 92])
    FOLLOW_exceptionHandler_in_exceptionGroup1401 = frozenset([1, 85, 86])
    FOLLOW_finallyClause_in_exceptionGroup1408 = frozenset([1])
    FOLLOW_finallyClause_in_exceptionGroup1416 = frozenset([1])
    FOLLOW_85_in_exceptionHandler1436 = frozenset([48])
    FOLLOW_ARG_ACTION_in_exceptionHandler1438 = frozenset([45])
    FOLLOW_ACTION_in_exceptionHandler1440 = frozenset([1])
    FOLLOW_86_in_finallyClause1470 = frozenset([45])
    FOLLOW_ACTION_in_finallyClause1472 = frozenset([1])
    FOLLOW_elementNoOptionSpec_in_element1494 = frozenset([1])
    FOLLOW_id_in_elementNoOptionSpec1505 = frozenset([71, 87])
    FOLLOW_71_in_elementNoOptionSpec1510 = frozenset([42, 43, 44, 49, 89, 92])
    FOLLOW_87_in_elementNoOptionSpec1514 = frozenset([42, 43, 44, 49, 89, 92])
    FOLLOW_atom_in_elementNoOptionSpec1517 = frozenset([1, 74, 90, 91])
    FOLLOW_ebnfSuffix_in_elementNoOptionSpec1523 = frozenset([1])
    FOLLOW_id_in_elementNoOptionSpec1582 = frozenset([71, 87])
    FOLLOW_71_in_elementNoOptionSpec1587 = frozenset([82])
    FOLLOW_87_in_elementNoOptionSpec1591 = frozenset([82])
    FOLLOW_block_in_elementNoOptionSpec1594 = frozenset([1, 74, 90, 91])
    FOLLOW_ebnfSuffix_in_elementNoOptionSpec1600 = frozenset([1])
    FOLLOW_atom_in_elementNoOptionSpec1659 = frozenset([1, 74, 90, 91])
    FOLLOW_ebnfSuffix_in_elementNoOptionSpec1665 = frozenset([1])
    FOLLOW_ebnf_in_elementNoOptionSpec1711 = frozenset([1])
    FOLLOW_ACTION_in_elementNoOptionSpec1718 = frozenset([1])
    FOLLOW_SEMPRED_in_elementNoOptionSpec1725 = frozenset([1, 88])
    FOLLOW_88_in_elementNoOptionSpec1729 = frozenset([1])
    FOLLOW_treeSpec_in_elementNoOptionSpec1748 = frozenset([1, 74, 90, 91])
    FOLLOW_ebnfSuffix_in_elementNoOptionSpec1754 = frozenset([1])
    FOLLOW_range_in_atom1806 = frozenset([1, 38, 39])
    FOLLOW_ROOT_in_atom1813 = frozenset([1])
    FOLLOW_BANG_in_atom1817 = frozenset([1])
    FOLLOW_terminal_in_atom1845 = frozenset([1])
    FOLLOW_notSet_in_atom1853 = frozenset([1, 38, 39])
    FOLLOW_ROOT_in_atom1860 = frozenset([1])
    FOLLOW_BANG_in_atom1864 = frozenset([1])
    FOLLOW_RULE_REF_in_atom1892 = frozenset([1, 38, 39, 48])
    FOLLOW_ARG_ACTION_in_atom1898 = frozenset([1, 38, 39])
    FOLLOW_ROOT_in_atom1908 = frozenset([1])
    FOLLOW_BANG_in_atom1912 = frozenset([1])
    FOLLOW_89_in_notSet1995 = frozenset([42, 43, 44, 82])
    FOLLOW_notTerminal_in_notSet2001 = frozenset([1])
    FOLLOW_block_in_notSet2015 = frozenset([1])
    FOLLOW_TREE_BEGIN_in_treeSpec2039 = frozenset([32, 37, 42, 43, 44, 45, 49, 82, 89, 92])
    FOLLOW_element_in_treeSpec2041 = frozenset([32, 37, 42, 43, 44, 45, 49, 82, 89, 92])
    FOLLOW_element_in_treeSpec2045 = frozenset([32, 37, 42, 43, 44, 45, 49, 82, 84, 89, 92])
    FOLLOW_84_in_treeSpec2050 = frozenset([1])
    FOLLOW_block_in_ebnf2082 = frozenset([1, 74, 88, 90, 91])
    FOLLOW_90_in_ebnf2090 = frozenset([1])
    FOLLOW_74_in_ebnf2107 = frozenset([1])
    FOLLOW_91_in_ebnf2124 = frozenset([1])
    FOLLOW_88_in_ebnf2141 = frozenset([1])
    FOLLOW_CHAR_LITERAL_in_range2224 = frozenset([13])
    FOLLOW_RANGE_in_range2226 = frozenset([44])
    FOLLOW_CHAR_LITERAL_in_range2230 = frozenset([1])
    FOLLOW_CHAR_LITERAL_in_terminal2261 = frozenset([1, 38, 39])
    FOLLOW_TOKEN_REF_in_terminal2283 = frozenset([1, 38, 39, 48])
    FOLLOW_ARG_ACTION_in_terminal2290 = frozenset([1, 38, 39])
    FOLLOW_STRING_LITERAL_in_terminal2329 = frozenset([1, 38, 39])
    FOLLOW_92_in_terminal2344 = frozenset([1, 38, 39])
    FOLLOW_ROOT_in_terminal2365 = frozenset([1])
    FOLLOW_BANG_in_terminal2386 = frozenset([1])
    FOLLOW_set_in_notTerminal0 = frozenset([1])
    FOLLOW_90_in_ebnfSuffix2446 = frozenset([1])
    FOLLOW_74_in_ebnfSuffix2458 = frozenset([1])
    FOLLOW_91_in_ebnfSuffix2471 = frozenset([1])
    FOLLOW_REWRITE_in_rewrite2500 = frozenset([32])
    FOLLOW_SEMPRED_in_rewrite2504 = frozenset([37, 40, 42, 43, 44, 45, 49, 82, 93])
    FOLLOW_rewrite_alternative_in_rewrite2508 = frozenset([40])
    FOLLOW_REWRITE_in_rewrite2516 = frozenset([37, 42, 43, 44, 45, 49, 82, 93])
    FOLLOW_rewrite_alternative_in_rewrite2520 = frozenset([1])
    FOLLOW_rewrite_template_in_rewrite_alternative2571 = frozenset([1])
    FOLLOW_rewrite_tree_alternative_in_rewrite_alternative2576 = frozenset([1])
    FOLLOW_82_in_rewrite_tree_block2618 = frozenset([37, 42, 43, 44, 45, 49, 82, 93])
    FOLLOW_rewrite_tree_alternative_in_rewrite_tree_block2620 = frozenset([84])
    FOLLOW_84_in_rewrite_tree_block2622 = frozenset([1])
    FOLLOW_rewrite_tree_element_in_rewrite_tree_alternative2656 = frozenset([1, 37, 42, 43, 44, 45, 49, 82, 93])
    FOLLOW_rewrite_tree_atom_in_rewrite_tree_element2684 = frozenset([1])
    FOLLOW_rewrite_tree_atom_in_rewrite_tree_element2689 = frozenset([74, 90, 91])
    FOLLOW_ebnfSuffix_in_rewrite_tree_element2691 = frozenset([1])
    FOLLOW_rewrite_tree_in_rewrite_tree_element2725 = frozenset([1, 74, 90, 91])
    FOLLOW_ebnfSuffix_in_rewrite_tree_element2731 = frozenset([1])
    FOLLOW_rewrite_tree_ebnf_in_rewrite_tree_element2777 = frozenset([1])
    FOLLOW_CHAR_LITERAL_in_rewrite_tree_atom2793 = frozenset([1])
    FOLLOW_TOKEN_REF_in_rewrite_tree_atom2800 = frozenset([1, 48])
    FOLLOW_ARG_ACTION_in_rewrite_tree_atom2802 = frozenset([1])
    FOLLOW_RULE_REF_in_rewrite_tree_atom2823 = frozenset([1])
    FOLLOW_STRING_LITERAL_in_rewrite_tree_atom2830 = frozenset([1])
    FOLLOW_93_in_rewrite_tree_atom2839 = frozenset([42, 49])
    FOLLOW_id_in_rewrite_tree_atom2841 = frozenset([1])
    FOLLOW_ACTION_in_rewrite_tree_atom2852 = frozenset([1])
    FOLLOW_rewrite_tree_block_in_rewrite_tree_ebnf2873 = frozenset([74, 90, 91])
    FOLLOW_ebnfSuffix_in_rewrite_tree_ebnf2875 = frozenset([1])
    FOLLOW_TREE_BEGIN_in_rewrite_tree2895 = frozenset([42, 43, 44, 45, 49, 93])
    FOLLOW_rewrite_tree_atom_in_rewrite_tree2897 = frozenset([37, 42, 43, 44, 45, 49, 82, 84, 93])
    FOLLOW_rewrite_tree_element_in_rewrite_tree2899 = frozenset([37, 42, 43, 44, 45, 49, 82, 84, 93])
    FOLLOW_84_in_rewrite_tree2902 = frozenset([1])
    FOLLOW_id_in_rewrite_template2934 = frozenset([82])
    FOLLOW_82_in_rewrite_template2938 = frozenset([42, 49, 84])
    FOLLOW_rewrite_template_args_in_rewrite_template2940 = frozenset([84])
    FOLLOW_84_in_rewrite_template2942 = frozenset([50, 51])
    FOLLOW_DOUBLE_QUOTE_STRING_LITERAL_in_rewrite_template2950 = frozenset([1])
    FOLLOW_DOUBLE_ANGLE_STRING_LITERAL_in_rewrite_template2956 = frozenset([1])
    FOLLOW_rewrite_template_ref_in_rewrite_template2983 = frozenset([1])
    FOLLOW_rewrite_indirect_template_head_in_rewrite_template2992 = frozenset([1])
    FOLLOW_ACTION_in_rewrite_template3001 = frozenset([1])
    FOLLOW_id_in_rewrite_template_ref3014 = frozenset([82])
    FOLLOW_82_in_rewrite_template_ref3018 = frozenset([42, 49, 84])
    FOLLOW_rewrite_template_args_in_rewrite_template_ref3020 = frozenset([84])
    FOLLOW_84_in_rewrite_template_ref3022 = frozenset([1])
    FOLLOW_82_in_rewrite_indirect_template_head3050 = frozenset([45])
    FOLLOW_ACTION_in_rewrite_indirect_template_head3052 = frozenset([84])
    FOLLOW_84_in_rewrite_indirect_template_head3054 = frozenset([82])
    FOLLOW_82_in_rewrite_indirect_template_head3056 = frozenset([42, 49, 84])
    FOLLOW_rewrite_template_args_in_rewrite_indirect_template_head3058 = frozenset([84])
    FOLLOW_84_in_rewrite_indirect_template_head3060 = frozenset([1])
    FOLLOW_rewrite_template_arg_in_rewrite_template_args3084 = frozenset([1, 81])
    FOLLOW_81_in_rewrite_template_args3087 = frozenset([42, 49])
    FOLLOW_rewrite_template_arg_in_rewrite_template_args3089 = frozenset([1, 81])
    FOLLOW_id_in_rewrite_template_arg3122 = frozenset([71])
    FOLLOW_71_in_rewrite_template_arg3124 = frozenset([45])
    FOLLOW_ACTION_in_rewrite_template_arg3126 = frozenset([1])
    FOLLOW_TOKEN_REF_in_id3147 = frozenset([1])
    FOLLOW_RULE_REF_in_id3157 = frozenset([1])
    FOLLOW_rewrite_template_in_synpred1_ANTLRv3_py2571 = frozenset([1])
    FOLLOW_rewrite_tree_alternative_in_synpred2_ANTLRv3_py2576 = frozenset([1])



def main(argv, stdin=sys.stdin, stdout=sys.stdout, stderr=sys.stderr):
    from antlr3.main import ParserMain
    main = ParserMain("ANTLRv3_pyLexer", ANTLRv3_pyParser)
    main.stdin = stdin
    main.stdout = stdout
    main.stderr = stderr
    main.execute(argv)


if __name__ == '__main__':
    main(sys.argv)
