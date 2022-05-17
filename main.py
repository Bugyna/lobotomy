# fucking lisp
# lob aka lot of brackets and also lobotomy because that's what it feels like programming in this
import string


class EXPR:
	def __init__(self, tokens):
		self.tokens = tokens
		self.value = None


	def evaluate(self):
		for t in self.tokens():
			pass

class TOKEN:
	def __init__(self, c, t):
		self.c = c
		self.t = t


	def __eq__(self, cond):
		if (self.t == "IDENTIFIER"):
			return self.t == cond
			
		return self.c == cond

	def __str__(self):
		if (self.c == "\n"): return f"{self.t}"
		return f"{self.c} : {self.t}"

	def __repr__(self):
		return self.__str__()


def tokenize(text):
	tokens = []
	token = ""
	number = False
	quote = False
	escaped = False
	for c in text:
		if (c == "("):
			if (token):
				if (number):
					token = int(token)
				tokens.append(TOKEN(token, "IDENTIFIER"))
				token = ""
				number = False
			tokens.append(TOKEN(c, "LPAREN"))

		elif (c == ")"):
			if (token):
				if (number):
					token = int(token)
				tokens.append(TOKEN(token, "IDENTIFIER"))
				token = ""
				number = False
			tokens.append(TOKEN(c, "RPAREN"))

		elif (c in string.ascii_letters):
			token += c

		elif (c in string.digits):
			if (token):
				token += c
			else:
				number = True
				token += c


		elif (c == "\\"):
			escaped = True

		elif (c == "\""):
			quote = not quote
			if (not quote):
				tokens.append(TOKEN(token, "STRING"))
				token = ""


		elif (c == " "):
			if (token):
				if (number):
					token = int(token)
				tokens.append(TOKEN(token, "IDENTIFIER"))
				token = ""
				number = False

		elif (c == "\n"):
			if (token):
				if (number):
					token = int(token)
				tokens.append(TOKEN(token, "IDENTIFIER"))
				token = ""
				number = False

			tokens.append(TOKEN(c, "NEWLINE"))

		# elif (c in "+-*/%"):


	return tokens


def empty_expr():
	return {
		"keyword" : "",
		"args" : [],
	}

def parse(tokens):

	expr = empty_expr()
	expr_ret = []

	p_count = 0
	i = 0
	line = 1
	column = 0
	args = False
	arg_p_count = None
	# print("parsing: ", tokens, len(tokens))
	while i < len(tokens):
		t = tokens[i]
	# for t in tokens:
		if (t == "("):
			if (args):
				# print("i before: ", i, tokens[i])
				tmp_expr, tmp_i = parse(tokens[i:])
				i += tmp_i - 1
				# print("i after: ", i, tokens[i])
				# print("tmp_expr: ", tmp_expr)
				expr["args"].append(tmp_expr)
				# print("after tmp: ", expr, p_count, arg_p_count)
				
			p_count += 1


		elif (t == ")"):
			p_count -= 1
			if (arg_p_count == p_count):
				arg_p_count = None
				args = False
				expr_ret.append(expr)
				expr = empty_expr()
				# print(expr)

			#if (p_count == 1):
				#expr_ret.append(expr)
				# print("happens", expr)
				# print("expr_ret: ", expr_ret)
				#expr = empty_expr()
				# break

			if (p_count == 0):
				if (expr["keyword"]):
					expr_ret.append(expr)
				# print("breaking: ", i)
				break


		elif (t.t == "STRING"):
			if (args):
				expr["args"].append(t.c)
			else:
				raise Exception(f"string not in function arguments {line}.{column}")


		elif (t == "\n"):
			column = 0
			line += 1


		elif (t == "IDENTIFIER"):
			if (args):
				expr["args"].append(t.c)

			else:
				expr["keyword"] = t.c
				args = True
				arg_p_count = p_count - 1


		if (p_count < 0):
			 raise Exception(f"too many right parenthesis {line}.{column} [{i}: {tokens[i].t}]")

		i += 1
		column += 1

	# if (p_count > 0):
		# print("pcount: ", p_count)
		# raise Exception(f"too many left parenthesis {line}.{column} [{i}: {tokens[i].t}]")

	# print("end: ", expr_ret)
	if (len(expr_ret) == 1): expr_ret = expr_ret[0]
	return expr_ret, i



def eval_expr(expr):
	# print("evaluating: ", expr)
	if (not expr["keyword"]):
		return None
	
	else:
		if (expr["args"]):
			i = 0
			while i < len(expr["args"]):
				if (type(expr["args"][i]) == dict):
					expr["args"][i] = eval_expr(expr["args"][i])

				elif (type(expr["args"][i]) == list):
					j = 0
					for arg in expr["args"][i]:
						if (type(arg) == dict):
							expr["args"][i][j] = eval_expr(arg)

						j += 1

				i += 1

			# print("args: ", expr["args"])

		if (expr["keyword"] in ["add", "minus", "mul", "div", "pow"]):
			return inbuilt_arithmetic(expr["keyword"], expr["args"])

		elif (expr["keyword"] in ["let"]):
			return create_var(expr)

		elif (expr["keyword"] == "print"):
			inbuilt_print(expr["args"])
				

	# for 
	# if (expr["keyword"] == "add"):
		# print(inbuilt_add(expr["args"]))
		# print(expr, inbuilt_add(expr["args"]))


def interpret(expr_list):
	print("expr_list: ", expr_list)
	for expr in expr_list:
		print("expr: ", expr)
		print(eval_expr(expr))


vars = {}

def create_var(expr):
	global vars
	if (type(expr["args"]) == list and len(expr["args"]) == 1):
		expr["args"] = expr["args"][0]

	vars[expr["args"][0]] = expr["args"][1:] if len(expr["args"][1:]) > 1 else expr["args"][1]
	print("created var", expr["args"][0], expr["args"][1:])


def inbuilt_arithmetic(keyword, args):
	global vars
	i = None
	print("inbuilt: ", keyword, args)
	for arg in args:
		if (not arg): arg = 0
		if (type(arg) == str and arg in vars):
			arg = vars[arg]

		if (not i):
			i = arg; continue

		if (keyword == "add"):
			i += arg

		elif (keyword == "minus"):
			i -= arg

		elif (keyword == "mul"):
			i *= arg

		elif (keyword == "div"):
			i /= arg

		elif (keyword == "pow"):
			i **= arg

	# print("inbuilt args: ", args, i)
	return i

def inbuilt_print(args):
	s = ""
	for arg in args:
		if (type(arg) == str and arg in vars):
			arg = vars[arg]

		s += str(arg) + " "
	print(s)

t = """
(
	(div 2 2)
	(
		add 2 
			(add 4 (add 4 2))

	)
	(minus 2 8)
	(mul 2 8)
	(pow 2 8)
	(let a 2)
	(minus a 4 22 (add 22 24))
)
"""

# t = """(
# 	(add t (2 (add (4 4))))
# 	(add n (3 (add (5 5))))
# )"""

t = """(
(let a "aaa")
(let b (add 2 2))
(print a 22 b)
)"""


print(t)
print("------------------------")

print("lex: ", tokenize(t))
print("------------------------")

print("parse: ", parse(tokenize(t)))
print("------------------------")

print(interpret(parse(tokenize(t))[0]))


