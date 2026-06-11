using Oscar

# Translation of the "M1+M2 example" and the "BIG physics Example" blocks
# from examples/example.lib. This recreates the module data in Oscar without
# the surrounding Singular test harness.

as_submodule(F, generators) = first(sub(F, generators))

function quad(k, n, c=30000)
    k <= 0 && throw(ArgumentError("k must be positive"))
    n <= 0 && throw(ArgumentError("n must be positive"))
    c <= 0 && throw(ArgumentError("c must be positive"))

    variable_names = [Symbol("x$i") for i in 1:n]
    R, x = polynomial_ring(QQ, variable_names; internal_ordering=:degrevlex)
    quadratic_monomials = [x[i] * x[j] for i in 1:n for j in i:n]

    function random_quadratic()
        polynomial = zero(R)
        while iszero(polynomial)
            polynomial = sum((rand(-c:c) * monomial for monomial in quadratic_monomials), init=zero(R))
        end
        return polynomial
    end

    return ideal(R, [random_quadratic() for _ in 1:k])
end

function embed_in_free_module(f, target_basis, offset)
    result = zero(parent(target_basis[1]))
    for i in 1:rank(parent(f))
        coeff = f[i]
        if !iszero(coeff)
            result += coeff * target_basis[offset + i]
        end
    end
    return result
end

function generators_from_matrix_rows(F, row_entries)
    e = basis(F)
    nrows = length(row_entries)
    ncols = length(first(row_entries))
    generators = [zero(F) for _ in 1:ncols]

    for j in 1:ncols
        g = zero(F)
        for i in 1:nrows
            coeff = row_entries[i][j]
            if !iszero(coeff)
                g += coeff * e[i]
            end
        end
        generators[j] = g
    end

    return generators
end

function map_polynomial_by_exponents(f, target_vars)
    S = parent(target_vars[1])
    result = zero(S)
    for (coeff, exponent_vector) in zip(coefficients(f), AbstractAlgebra.exponent_vectors(f))
        monomial = prod((target_vars[i]^exponent_vector[i] for i in 1:length(exponent_vector)), init=one(S))
        result += coeff * monomial
    end
    return result
end

function has_rational_function_coefficients(R)
    K = coefficient_ring(R)
    try
        symbols(K)
        base_ring(K)
        numerator(one(K))
        denominator(one(K))
        return true
    catch
        return false
    end
end

function module_to_ideal(M; flatten_coefficients=:auto)
    F = ambient_free_module(M)
    R = base_ring(F)
    module_rank = rank(F)
    module_generators = ambient_representatives_generators(M)
    should_flatten = flatten_coefficients == :auto ? has_rational_function_coefficients(R) : flatten_coefficients

    if should_flatten
        K = coefficient_ring(R)
        coefficient_symbols = symbols(K)
        extended_symbols = vcat(coefficient_symbols, symbols(R), [Symbol("e$i") for i in 1:module_rank])
        S, _ = polynomial_ring(base_ring(K), extended_symbols; internal_ordering=:degrevlex)
        S_gens = gens(S)
        coefficient_vars = S_gens[1:length(coefficient_symbols)]
        R_vars = S_gens[(length(coefficient_symbols) + 1):(length(coefficient_symbols) + ngens(R))]
        e = S_gens[(length(coefficient_symbols) + ngens(R) + 1):end]

        function coefficient_denominator(g)
            denominator_product = one(parent(numerator(one(K))))
            for component in 1:module_rank
                for coeff in coefficients(g[component])
                    denominator_product *= denominator(coeff)
                end
            end
            return denominator_product
        end

        function convert_component(f, common_denominator)
            result = zero(S)
            for (coeff, exponent_vector) in zip(coefficients(f), AbstractAlgebra.exponent_vectors(f))
                scaled_coefficient = numerator(coeff) * divexact(common_denominator, denominator(coeff))
                mapped_coefficient = map_polynomial_by_exponents(scaled_coefficient, coefficient_vars)
                monomial = prod((R_vars[i]^exponent_vector[i] for i in 1:length(exponent_vector)), init=one(S))
                result += mapped_coefficient * monomial
            end
            return result
        end

        ideal_generators = [
            let common_denominator = coefficient_denominator(g)
                sum((convert_component(g[i], common_denominator) * e[i] for i in 1:module_rank), init=zero(S))
            end
            for g in module_generators
        ]
        append!(ideal_generators, [e[i] * e[j] for i in 1:module_rank for j in i:module_rank])

        return ideal(S, ideal_generators)
    end

    extended_symbols = vcat(symbols(R), [Symbol("e$i") for i in 1:module_rank])
    S, _ = polynomial_ring(coefficient_ring(R), extended_symbols; internal_ordering=:degrevlex)
    S_gens = gens(S)
    R_to_S = hom(R, S, S_gens[1:ngens(R)], check=false)
    e = S_gens[(ngens(R) + 1):end]

    ideal_generators = [
        sum((R_to_S(g[i]) * e[i] for i in 1:module_rank), init=zero(S))
        for g in module_generators
    ]
    append!(ideal_generators, [e[i] * e[j] for i in 1:module_rank for j in i:module_rank])

    return ideal(S, ideal_generators)
end

function small_physics_example()
    K, (s, t) = rational_function_field(QQ, [:s, :t])
    R, (z1, z2, z3, z4, z5, z6, z7, z8, z9, z10, z11, z12, z13, z14, z15) = polynomial_ring(
        K,
        [:z1, :z2, :z3, :z4, :z5, :z6, :z7, :z8, :z9, :z10, :z11, :z12, :z13, :z14, :z15],
    )

    m1_rows = [
        [-z1 + z2, -z1 + z2, -z1 + z2 - z8 + z11, -z1 + z2 - z6 + z14, 0, 0, 0, 0, 0, 0, 0, -s - z1 + z2, -t - z1 + z2, 0, 0],
        [0, 0, z1 - z2 + z8 - z11, 0, -z6 + z8 - z11 + z14, 0, 0, z8 - z11, s + z8 - z11, s + t + z8 - z11, z8 - z11, 0, 0, 0, 0],
        [0, 0, 0, z1 - z2 + z6 - z14, z6 - z8 + z11 - z14, z6 - z14, s + t + z6 - z14, 0, 0, 0, 0, 0, 0, z6 - z14, -s + z6 - z14],
        [z1 - z12, s + z1 - z12, s + z1 + z8 - z9 - z12, z1 - z6 - z12 + z15, 0, 0, 0, 0, 0, 0, 0, z1 - z12, s + t + z1 - z12, 0, 0],
        [0, 0, -s - z1 - z8 + z9 + z12, 0, -s - z6 - z8 + z9 + z15, 0, 0, -z8 + z9, -z8 + z9, -s - t - z8 + z9, -s - z8 + z9, 0, 0, 0, 0],
        [0, 0, 0, -z1 + z6 + z12 - z15, s + z6 + z8 - z9 - z15, z6 - z15, -t + z6 - z15, 0, 0, 0, 0, 0, 0, -s + z6 - z15, z6 - z15],
        [z1 - z13, t + z1 - z13, -s + z1 + z9 - z10 - z13, z1 + 3 * z6 - z7 - z13 - z14 - z15, 0, 0, 0, 0, 0, 0, 0, s + t + z1 - z13, z1 - z13, 0, 0],
        [0, 0, s - z1 - z9 + z10 + z13, 0, s + 3 * z6 - z7 - z9 + z10 - z14 - z15, 0, 0, s + t - z9 + z10, -z9 + z10, -z9 + z10, s - z9 + z10, 0, 0, 0, 0],
        [0, 0, 0, -z1 - 3 * z6 + z7 + z13 + z14 + z15, -s - 3 * z6 + z7 + z9 - z10 + z14 + z15, -3 * z6 + z7 + z14 + z15, -s - 3 * z6 + z7 + z14 + z15, 0, 0, 0, 0, 0, 0, -t - 3 * z6 + z7 + z14 + z15, s + t - 3 * z6 + z7 + z14 + z15],
        [2 * z1, z1 + z2, z1 + z3 - z11, z1 + z4 - z6, 0, 0, 0, 0, 0, 0, 0, z1 + z12, z1 + z13, 0, 0],
        [0, 0, -z1 - z3 + z11, 0, -z3 + z4 - z6 + z11, 0, 0, z2 - z3 + z11, z1 + z2 - z3 + z11 - z12, 2 * z1 + z2 - z3 + z11 - z12 - z13, z1 - z3 + z11, 0, 0, 0, 0],
        [0, 0, 0, -z1 - z4 + z6, z3 - z4 + z6 - z11, z1 - z4 + z6, 2 * z1 + z2 - z4 + z6 - z12 - z13, 0, 0, 0, 0, 0, 0, 2 * z1 - z2 - z4 + z6, -z4 + z6 + z12],
        [z1 - z3 + z11, z1 - z3 + z8, z1 - z3 - z11, z1 - z3 + z5 - z6, 0, 0, 0, 0, 0, 0, 0, s + z1 - z3 + z8 - z9 + z11, -s + z1 - z3 + z9 - z10 + z11, 0, 0],
        [0, 0, -z1 + z3 + z11, 0, z5 - z6 + z11, 0, 0, z8 + z11, -s + z9 + z11, z10 + z11, 2 * z11, 0, 0, 0, 0],
        [0, 0, 0, -z1 + z3 - z5 + z6, -z5 + z6 - z11, -z5 + z6 + z11, -z5 + z6 + z10, 0, 0, 0, 0, 0, 0, -z5 + z6 - z8 + 2 * z11, s - z5 + z6 + z8 - z9 + z11],
        [z1 - z4 + z6, z1 - z4 + 2 * z6 - z14, z1 - z4 + z5 - z11, z1 - z4 - z6, 0, 0, 0, 0, 0, 0, 0, z1 - z4 + z15, z1 - z4 + 4 * z6 - z7 - z14 - z15, 0, 0],
        [0, 0, -z1 + z4 - z5 + z11, 0, -z5 - z6 + z11, 0, 0, -z5 + 2 * z6 + z11 - z14, -z5 + 3 * z6 + z11 - z14 - z15, -z5 + z7 + z11, -z5 + z6 + z11, 0, 0, 0, 0],
        [0, 0, 0, -z1 + z4 + z6, z5 + z6 - z11, 2 * z6, z6 + z7, 0, 0, 0, 0, 0, 0, z6 + z14, z6 + z15],
    ]

    m2_rows = [
        [z1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, z2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, z3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, z4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, z5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, z6, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, z7, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, z8, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, z9, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, z10, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    ]

    F = free_module(R, 15)
    m1_gens = generators_from_matrix_rows(F, m1_rows)
    m2_gens = generators_from_matrix_rows(F, m2_rows)

    m1 = as_submodule(F, m1_gens)
    m2 = as_submodule(F, m2_gens)
    m = m1 + m2

    F_intersect = free_module(R, 30)
    e_intersect = basis(F_intersect)
    m1_left = as_submodule(F_intersect, [embed_in_free_module(g, e_intersect, 0) for g in m1_gens])
    m2_diag = as_submodule(
        F_intersect,
        [embed_in_free_module(g, e_intersect, 0) + embed_in_free_module(g, e_intersect, 15) for g in m2_gens],
    )
    m_intersect = m1_left + m2_diag

    return (
        R = R,
        m1 = m1,
        m2 = m2,
        m = m,
        m_intersect = m_intersect,
    )
end

function generator_from_row(F, row)
    e = basis(F)
    result = zero(F)
    for i in 1:length(row)
        coeff = row[i]
        if coeff != 0
            result += coeff * e[i]
        end
    end
    return result
end

function print_example_summary(name, ex)
    println("Constructed ", name, " in Oscar")
    println("base ring: ", ex.R)
    println("ngens(m1): ", ngens(ex.m1))
    println("ngens(m2): ", ngens(ex.m2))
    println("ngens(m): ", ngens(ex.m))
    println("ngens(m_intersect): ", ngens(ex.m_intersect))
    println("ambient rank(m_intersect): ", rank(ambient_free_module(ex.m_intersect)))
end

function m1_plus_m2_example()
    K, (s, t) = rational_function_field(QQ, [:s, :t])
    R, (z1, z2, z3, z4, z5, z6, z7, z8, z9, z10, z11, z12, z13, z14, z15) = polynomial_ring(
        K,
        [:z1, :z2, :z3, :z4, :z5, :z6, :z7, :z8, :z9, :z10, :z11, :z12, :z13, :z14, :z15],
    )

    F = free_module(R, 15)

    m1_rows = [
        [-z1 + z2, -z1 + z2, -z1 + z2 - z8 + z11, -z1 + z2 - z6 + z14, 0, 0, 0, 0, 0, 0, 0, -s - z1 + z2, -t - z1 + z2, 0, 0],
        [0, 0, z1 - z2 + z8 - z11, 0, -z6 + z8 - z11 + z14, 0, 0, z8 - z11, s + z8 - z11, s + t + z8 - z11, z8 - z11, 0, 0, 0, 0],
        [0, 0, 0, z1 - z2 + z6 - z14, z6 - z8 + z11 - z14, z6 - z14, s + t + z6 - z14, 0, 0, 0, 0, 0, 0, z6 - z14, -s + z6 - z14],
        [z1 - z12, s + z1 - z12, s + z1 + z8 - z9 - z12, z1 - z6 - z12 + z15, 0, 0, 0, 0, 0, 0, 0, z1 - z12, s + t + z1 - z12, 0, 0],
        [0, 0, -s - z1 - z8 + z9 + z12, 0, -s - z6 - z8 + z9 + z15, 0, 0, -z8 + z9, -z8 + z9, -s - t - z8 + z9, -s - z8 + z9, 0, 0, 0, 0],
        [0, 0, 0, -z1 + z6 + z12 - z15, s + z6 + z8 - z9 - z15, z6 - z15, -t + z6 - z15, 0, 0, 0, 0, 0, 0, -s + z6 - z15, z6 - z15],
        [z1 - z13, t + z1 - z13, -s + z1 + z9 - z10 - z13, z1 + 3 * z6 - z7 - z13 - z14 - z15, 0, 0, 0, 0, 0, 0, 0, s + t + z1 - z13, z1 - z13, 0, 0],
        [0, 0, s - z1 - z9 + z10 + z13, 0, s + 3 * z6 - z7 - z9 + z10 - z14 - z15, 0, 0, s + t - z9 + z10, -z9 + z10, -z9 + z10, s - z9 + z10, 0, 0, 0, 0],
        [0, 0, 0, -z1 - 3 * z6 + z7 + z13 + z14 + z15, -s - 3 * z6 + z7 + z9 - z10 + z14 + z15, -3 * z6 + z7 + z14 + z15, -s - 3 * z6 + z7 + z14 + z15, 0, 0, 0, 0, 0, 0, -t - 3 * z6 + z7 + z14 + z15, s + t - 3 * z6 + z7 + z14 + z15],
        [2 * z1, z1 + z2, z1 + z3 - z11, z1 + z4 - z6, 0, 0, 0, 0, 0, 0, 0, z1 + z12, z1 + z13, 0, 0],
        [0, 0, -z1 - z3 + z11, 0, -z3 + z4 - z6 + z11, 0, 0, z2 - z3 + z11, z1 + z2 - z3 + z11 - z12, 2 * z1 + z2 - z3 + z11 - z12 - z13, z1 - z3 + z11, 0, 0, 0, 0],
        [0, 0, 0, -z1 - z4 + z6, z3 - z4 + z6 - z11, z1 - z4 + z6, 2 * z1 + z2 - z4 + z6 - z12 - z13, 0, 0, 0, 0, 0, 0, 2 * z1 - z2 - z4 + z6, -z4 + z6 + z12],
        [z1 - z3 + z11, z1 - z3 + z8, z1 - z3 - z11, z1 - z3 + z5 - z6, 0, 0, 0, 0, 0, 0, 0, s + z1 - z3 + z8 - z9 + z11, -s + z1 - z3 + z9 - z10 + z11, 0, 0],
        [0, 0, -z1 + z3 + z11, 0, z5 - z6 + z11, 0, 0, z8 + z11, -s + z9 + z11, z10 + z11, 2 * z11, 0, 0, 0, 0],
        [0, 0, 0, -z1 + z3 - z5 + z6, -z5 + z6 - z11, -z5 + z6 + z11, -z5 + z6 + z10, 0, 0, 0, 0, 0, 0, -z5 + z6 - z8 + 2 * z11, s - z5 + z6 + z8 - z9 + z11],
        [z1 - z4 + z6, z1 - z4 + 2 * z6 - z14, z1 - z4 + z5 - z11, z1 - z4 - z6, 0, 0, 0, 0, 0, 0, 0, z1 - z4 + z15, z1 - z4 + 4 * z6 - z7 - z14 - z15, 0, 0],
        [0, 0, -z1 + z4 - z5 + z11, 0, -z5 - z6 + z11, 0, 0, -z5 + 2 * z6 + z11 - z14, -z5 + 3 * z6 + z11 - z14 - z15, -z5 + z7 + z11, -z5 + z6 + z11, 0, 0, 0, 0],
        [0, 0, 0, -z1 + z4 + z6, z5 + z6 - z11, 2 * z6, z6 + z7, 0, 0, 0, 0, 0, 0, z6 + z14, z6 + z15],
    ]

    m2_rows = [
        [z1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, z2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, z3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, z4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, z5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, z6, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, z7, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, z8, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, z9, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, z10, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    ]

    m1_gens = [generator_from_row(F, row) for row in m1_rows]
    m2_gens = [generator_from_row(F, row) for row in m2_rows]

    m1 = as_submodule(F, m1_gens)
    m2 = as_submodule(F, m2_gens)
    m = m1 + m2

    F_intersect = free_module(R, 30)
    e_intersect = basis(F_intersect)
    m1_left = as_submodule(F_intersect, [embed_in_free_module(g, e_intersect, 0) for g in m1_gens])
    m2_diag = as_submodule(
        F_intersect,
        [embed_in_free_module(g, e_intersect, 0) + embed_in_free_module(g, e_intersect, 15) for g in m2_gens],
    )
    m_intersect = m1_left + m2_diag

    return (
        R = R,
        m1 = m1,
        m2 = m2,
        m = m,
        m_intersect = m_intersect,
    )
end

small_physics_example() = m1_plus_m2_example()

function big_physics_example()
    R, (c1, c2, c3, c4, z8, z9, z1, z2, z3, z4, z5, z6, z7) = polynomial_ring(
        QQ,
        [:c1, :c2, :c3, :c4, :z8, :z9, :z1, :z2, :z3, :z4, :z5, :z6, :z7],
    )

    F = free_module(R, 9)
    e = basis(F)

    m1_original_gens = [
        (-z1 + z4) * e[1] +
        (3 * c1 + c4 - z1 + 2 * z2 + z4 - z6 - z9) * e[3] +
        (-z1 + z4) * e[4] +
        (4 * c1 - z1 + 2 * z2 + z4 - z6 - z9) * e[5] +
        (c2 - c3 - z1 + z4) * e[8],

        (3 * c1 + c4 + 2 * z2 - z6 - z9) * e[2] +
        (3 * c1 + c4 - z1 + 2 * z2 + z4 - z6 - z9) * e[3] +
        (4 * c1 - z1 + 2 * z2 + z4 - z6 - z9) * e[5] +
        (4 * c1 + 2 * z2 - z6 - z9) * e[6] +
        (3 * c1 - c2 + c3 + c4 + 2 * z2 - z6 - z9) * e[7] +
        (2 * c1 + 2 * c4 + 2 * z2 - z6 - z9) * e[9],

        (-3 * c1 - c3 - z1 - z2 - z3 + z5 + z8 + z9) * e[1] +
        (-3 * c1 + c2 - c3 - c4 - z1 - z2 - z3 + z5 + z6 - z7 + z8 + z9) * e[3] +
        (-2 * c1 + c2 - 2 * c3 - c4 - z1 - z2 - z3 + z5 + z8 + z9) * e[4] +
        (-2 * c1 + c2 - 2 * c3 - c4 - z1 - z2 - z3 + z5 + z6 - z7 + z8 + z9) * e[5] +
        (-4 * c1 - z1 - z2 - z3 + z5 + z8 + z9) * e[8],

        (c2 - c4 + z6 - z7) * e[2] +
        (-3 * c1 + c2 - c3 - c4 - z1 - z2 - z3 + z5 + z6 - z7 + z8 + z9) * e[3] +
        (-2 * c1 + c2 - 2 * c3 - c4 - z1 - z2 - z3 + z5 + z6 - z7 + z8 + z9) * e[5] +
        (z6 - z7) * e[6] +
        (z6 - z7) * e[7] +
        (-c1 + c2 + c3 - c4 + z6 - z7) * e[9],

        (-c1 - c3 - 2 * z1 + z4 + z8) * e[1] +
        (c2 - c3 - 2 * z1 + z2 + z4 - z7 + z8) * e[3] +
        (-c1 + c2 - 2 * c3 - 2 * z1 + z4 + z8) * e[4] +
        (c2 - 2 * c3 - c4 - 2 * z1 + z2 + z4 - z7 + z8) * e[5] +
        (-c1 + c2 - 2 * z1 + z4 + z8) * e[8],

        (c1 + c2 + z2 - z7) * e[2] +
        (c2 - c3 - 2 * z1 + z2 + z4 - z7 + z8) * e[3] +
        (c2 - 2 * c3 - c4 - 2 * z1 + z2 + z4 - z7 + z8) * e[5] +
        (c1 - c4 + z2 - z7) * e[6] +
        (c1 - c2 + z2 - z7) * e[7] +
        (c1 + c2 + c3 + c4 + z2 - z7) * e[9],

        2 * (c1 + z1) * e[1] +
        (z1 - z2 + z3) * e[3] +
        (2 * c1 + z1 + z4) * e[4] +
        (-2 * c1 + z1 - 2 * z2 + z5 + z9) * e[5] +
        (c1 - c3 + z1 + z8) * e[8],

        (-2 * c1 - z1 - z2 + z3) * e[2] +
        (z1 - z2 + z3) * e[3] +
        (-2 * c1 + z1 - 2 * z2 + z5 + z9) * e[5] +
        (-4 * c1 - 2 * z2 - z4 + z5 + z9) * e[6] +
        (-c1 + c3 + z1 - z2 + z3 - z4 - z8) * e[7] +
        (-z1 + 2 * z3 - z5 - z9) * e[9],

        (-2 * c1 - z1 - z2 + z3) * e[1] +
        (-z1 + z2 + z3) * e[3] +
        (c1 + c4 - z1 + z2 + z3 - z6 - z9) * e[4] +
        (2 * c1 - z1 + 2 * z2 + z3 - z9) * e[5] +
        (-4 * c1 + c2 - c4 - z1 - 2 * z2 + z3 + z6 - z7 + z9) * e[8],

        2 * (c1 + z2) * e[2] +
        (-z1 + z2 + z3) * e[3] +
        (2 * c1 - z1 + 2 * z2 + z3 - z9) * e[5] +
        (c1 - c4 + z2 + z6) * e[6] +
        (c1 - c2 + z2 + z7) * e[7] +
        (z2 + z9) * e[9],
    ]

    m2_gens = [
        z1 * e[1],
        z2 * e[2],
        z3 * e[3],
        z4 * e[4],
        z5 * e[5],
        z6 * e[6],
        z7 * e[7],
        e[8],
        e[9],
    ]

    m1_gens = m1_original_gens

    m1 = as_submodule(F, m1_gens)
    m2 = as_submodule(F, m2_gens)
    m = m1 + m2

    F_intersect = free_module(R, 18)
    e_intersect = basis(F_intersect)
    m1_left = as_submodule(F_intersect, [embed_in_free_module(g, e_intersect, 0) for g in m1_gens])
    m2_diag = as_submodule(
        F_intersect,
        [embed_in_free_module(g, e_intersect, 0) + embed_in_free_module(g, e_intersect, 9) for g in m2_gens],
    )
    m_intersect = m1_left + m2_diag

    return (
        R = R,
        m2 = m2,
        m1 = m1,
        m = m,
        m_intersect = m_intersect,
    )
end

if abspath(PROGRAM_FILE) == @__FILE__
    print_example_summary("M1+M2 example", m1_plus_m2_example())
    println()
    print_example_summary("BIG physics Example", big_physics_example())

    # For the actual Buchberger step in Oscar, use for example:
    # ex = big_physics_example()
    # G = standard_basis(ex.m)
end