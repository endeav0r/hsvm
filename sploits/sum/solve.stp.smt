(set-logic QF_BV)
(set-info :smt-lib-version 2.0)

(define-fun ROTL3 ((x (_ BitVec 16))) (_ BitVec 16)
	(concat
		((_ extract 12 0) x)
		((_ extract 15 13) x)
	)
)

(define-fun ROTL7 ((x (_ BitVec 16))) (_ BitVec 16)
	(concat
		((_ extract 8 0) x)
		((_ extract 15 9) x)
	)
)

; using arrays greatly slows down computation
(declare-fun A0 () (_ BitVec 16))
(declare-fun B0 () (_ BitVec 16))
(declare-fun C0 () (_ BitVec 16))
(declare-fun D0 () (_ BitVec 16))
(declare-fun A1 () (_ BitVec 16))
(declare-fun B1 () (_ BitVec 16))
(declare-fun C1 () (_ BitVec 16))
(declare-fun D1 () (_ BitVec 16))
(declare-fun A2 () (_ BitVec 16))
(declare-fun B2 () (_ BitVec 16))
(declare-fun C2 () (_ BitVec 16))
(declare-fun D2 () (_ BitVec 16))
(declare-fun A3 () (_ BitVec 16))
(declare-fun B3 () (_ BitVec 16))
(declare-fun C3 () (_ BitVec 16))
(declare-fun D3 () (_ BitVec 16))

(declare-fun M0 () (_ BitVec 16))
(declare-fun M1 () (_ BitVec 16))
(declare-fun M2 () (_ BitVec 16))
(declare-fun M3 () (_ BitVec 16))

; initialize values
(assert (= A0 #xdead))
(assert (= B0 #xbeef))
(assert (= C0 #xf0f0))
(assert (= D0 #x0f0f))

;(assert (= M0 #x0123))
;(assert (= M1 #x4567))
;(assert (= M2 #x89ab))
;(assert (= M3 #xcdef))

; desired output goes here
(assert (= A3 #x0000))
(assert (= B3 #x0000))
(assert (= C3 #x0000))
(assert (= D3 #x0000))

;;;;;;;;;;;;;;;;;;;;;;;;;
;; ROUND 1
;;;;;;;;;;;;;;;;;;;;;;;;;
(assert
	(=
		A1
		(bvxor
			(bvadd
				(ROTL3 D0)
				(bvxor
					(bvor A0 B0)
					C0
				)
			)
			M0
		)
	)
)

(assert
	(=
		B1
		(bvxor
			(bvadd
				(ROTL7 A1)
				(bvxor
					(bvor B0 C0)
					D0
				)
			)
			M1
		)
	)
)

(assert
	(=
		C1
		(bvxor
			(bvadd
				(ROTL3 B1)
				(bvxor
					(bvor C0 D0)
					A1
				)
			)
			M2
		)
	)
)

(assert
	(=
		D1
		(bvxor
			(bvadd
				(ROTL7 C1)
				(bvxor
					(bvor D0 A1)
					B1
				)
			)
			M3
		)
	)
)

;;;;;;;;;;;;;;;;;;;;;;;;;
;; ROUND 2
;;;;;;;;;;;;;;;;;;;;;;;;;
(assert
	(=
		A2
		(bvxor
			(bvadd
				(ROTL3 D1)
				(bvxor
					(bvand A1 B1)
					C1
				)
			)
			M3
		)
	)
)

(assert
	(=
		B2
		(bvxor
			(bvadd
				(ROTL3 A2)
				(bvxor
					(bvand B1 C1)
					D1
				)
			)
			M2
		)
	)
)

(assert
	(=
		C2
		(bvxor
			(bvadd
				(ROTL7 B2)
				(bvxor
					(bvand C1 D1)
					A2
				)
			)
			M1
		)
	)
)

(assert
	(=
		D2
		(bvxor
			(bvadd
				(ROTL7 C2)
				(bvxor
					(bvand D1 A2)
					B2
				)
			)
			M0
		)
	)
)

;;;;;;;;;;;;;;;;;;;;;;;;;
;; ROUND 3
;;;;;;;;;;;;;;;;;;;;;;;;;
(assert
	(=
		A3
		(bvxor
			(bvadd
				(ROTL7 D2)
				(bvxor
					(bvxor A2 B2)
					C2
				)
			)
			M1
		)
	)
)

(assert
	(=
		B3
		(bvxor
			(bvadd
				(ROTL7 A3)
				(bvxor
					(bvxor B2 C2)
					D2
				)
			)
			M3
		)
	)
)

(assert
	(=
		C3
		(bvxor
			(bvadd
				(ROTL3 B3)
				(bvxor
					(bvxor C2 D2)
					A3
				)
			)
			M0
		)
	)
)

(assert
	(=
		D3
		(bvxor
			(bvadd
				(ROTL3 C3)
				(bvxor
					(bvxor D2 A3)
					B3
				)
			)
			M2
		)
	)
)

(check-sat)
;(get-model)