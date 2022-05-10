# fucking lisp
# lob aka lot of brackets and also lobotomy because that's what it feels like programming in this
import string


class TOKEN:
	def __init__(self, c, type):
		self.c = c
		self.type = type


	def __eq__(self, cond):
		if (self.type == "IDENTIFIER"):
			return self.type == cond
			
		return self.c == cond

	def __str__(self):
		# return f"{self.c} : {self.type}"
		# if (self.type == "IDENTIFIER"): return f"{self.c} : {self.type}"
		if (self.c == "\n"): return f"{self.type}"
		return f"{self.c} : {self.type}"

	def __repr__(self):
		return self.__str__()


def tokenize(text):
	tokens = []
	token = ""
	number = False
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
		"specifier" : "",
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
				
			elif (expr["keyword"]):
				arg_p_count = p_count
				args = True

			p_count += 1


		elif (t == ")"):
			p_count -= 1
			# print("aaa: ", expr, p_count)
			if (arg_p_count == p_count):
				# print("aaaa", arg_p_count, p_count, line, column, i, expr)
				arg_p_count = None
				args = False
				# print(expr)

			if (p_count == 1):
				if (type(expr["args"]) == list and len(expr["args"]) == 1):
					expr["args"] = expr["args"][0]
				expr_ret.append(expr)
				# print("happens", expr)
				# print("expr_ret: ", expr_ret)
				expr = empty_expr()
				# break

			elif (p_count == 0):
				if (expr["keyword"]):
					expr_ret.append(expr)
				# print("breaking: ", i)
				break


		elif (t == "\n"):
			column = 0
			line += 1


		elif (t == "IDENTIFIER"):
			if (args):
				expr["args"].append(t.c)

			else:
				if (expr["keyword"]):
					expr["specifier"] = t.c

				else:
					expr["keyword"] = t.c

		if (p_count < 0):
			 raise Exception(f"too many right parenthesis {line}.{column} [{i}: {tokens[i].type}]")

		i += 1
		column += 1

	# if (p_count > 0):
		# print("pcount: ", p_count)
		# raise Exception(f"too many left parenthesis {line}.{column} [{i}: {tokens[i].type}]")

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
				

	# for 
	# if (expr["keyword"] == "add"):
		# print(inbuilt_add(expr["args"]))
		# print(expr, inbuilt_add(expr["args"]))


def interpret(expr_list):
	for expr in expr_list:
		print("expr: ", expr)
		print(eval_expr(expr))


vars = {}

def create_var(expr):
	global vars
	vars[expr["specifier"]] = expr["args"]
	print("created var", expr["specifier"], expr["args"])

def inbuilt_arithmetic(keyword, args):
	i = None
	print("inbuilt: ", keyword, args)
	for arg in args:
		if (not arg): arg = 0
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

t = """
(
	(div (2 2))
	(
		add t (2 
			(add (4 (add (4 2))))

		)
	)
	(minus (2 8))
	(mul (2 8))
	(pow (2 8))
	(let a (2))
)
"""

# t = """(
# 	(add t (2 (add (4 4))))
# 	(add n (3 (add (5 5))))
# )"""

print(t)
print("------------------------")

print("lex: ", tokenize(t))
print("------------------------")

print("parse: ", parse(tokenize(t)))
print("------------------------")

print(interpret(parse(tokenize(t))[0]))