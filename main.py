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
	print("parsing: ", tokens, len(tokens))
	while i < len(tokens):
		t = tokens[i]
	# for t in tokens:
		if (t == "("):
			if (args):
				print("i before: ", i, tokens[i])
				tmp_expr, tmp_i = parse(tokens[i:])
				i += tmp_i - 1
				print("i after: ", i, tokens[i])
				print("tmp_expr: ", tmp_expr)
				expr["args"].append(tmp_expr)
				print("after tmp: ", expr, p_count, arg_p_count)
				
			elif (expr["keyword"]):
				arg_p_count = p_count
				args = True

			p_count += 1


		elif (t == ")"):
			p_count -= 1
			# print("aaa: ", expr, p_count)
			if (arg_p_count == p_count):
				print("aaaa", arg_p_count, p_count, line, column, i, expr)
				arg_p_count = None
				args = False
				# print(expr)

			if (p_count == 1):
				args = False
				arg_p_count = None
				expr_ret.append(expr)
				print("happens", expr)
				# print("expr_ret: ", expr_ret)
				expr = empty_expr()
				# break

			elif (p_count == 0):
				expr_ret.append(expr)
				print("breaking: ", i)
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

	print("end: ", expr_ret)
	if (len(expr_ret) == 1): expr_ret = expr_ret[0]
	return expr_ret, i



def eval_expr(expr):
	pass
	# for 
	# if (expr["keyword"] == "add"):
		# print(inbuilt_add(expr["args"]))
		# print(expr, inbuilt_add(expr["args"]))


def inbuilt_add(args):
	return sum(args)


t = """(
	(minus (2 2))
	(add t (2 (add (4 4)))
	(minus (2 2))

)"""


# t = """(
	# (add t (2 (add (4 4))))
	# (add n (3 (add (5 5))))
# )"""

print(t)
print("lex: ", tokenize(t))
print("parse: ", parse(tokenize(t)))